#include <swilib.h>
#include "../../../libsiemens/mplayer.h"
#include "../../../libsiemens/graphics.h"
#include "../../../libsiemens/lang.h"
#include "../../../libsiemens/other.h"
#include "../../../libsiemens/obs.h"
#include "../../../libshell/other.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/skin_utils.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/config_coord.h"
#include "conf_loader.h"
#include "config_data.h"

#define MP_STOP  0
#define MP_PAUSE 1
#define MP_PLAY  2

char *RamVolumeStep()
__swi(0x81Fa);

GBSTMR tmr;

enum
{
	lgpHeader,
	lgpSoftkeyLeft_open,
	lgpSoftkeyLeft_turn,
	lgpSoftkeyRight_close,
	lgpMain
};

char **lgp;
WSHDR *ws_title, *ws_artist, *ws_album, *ws_fname, *ws;

enum
{
	iconNext,
	iconNext_active,
	iconPause,
	iconPause_active,
	iconPlay,
	iconPlay_active,
	iconPrev,
	iconPrev_active,
	iconVol_up,
	iconVol_up_active,
	iconVol_down,
	iconVol_down_active,
	iconCover_bg,
	iconsTotal
};

IMGHDR *icons[iconsTotal];
IMGHDR *cover, *cover_bg;

//char covers_dir[128];
unsigned int player_csm_id;
unsigned int cur_pos = 1;
//ибо GetPlayStatus работает медленно
unsigned int play_status;

unsigned int *desk_id_ptr;
unsigned int plugin_id;

int GetCoverPath(char *dest, char *album)
{
	FSTATS fs;
	unsigned int err;
	enum {size = 4};
	char *ext[size] = {".png", ".jpeg", ".jpg", ".bmp"};
	for (int i = 0; i < size; i++)
	{
		sprintf(dest, "%s%s%s", cfg_covers_directory, album, ext[i]);
		if (GetFileStats(dest, &fs, &err) != -1) return 1;
	}
	return -1;
}


void SetSongInfo(void)
{
	if (player_csm_id)
	{
		CSM_RAM *csmp;
		csmp = FindCSMbyID(player_csm_id);
		if (csmp)
		{
#ifdef ELKA
			WSHDR **wws=((WSHDR ***)csmp)[(0x18C/4)]-1;
#else
	    	WSHDR **wws=((WSHDR ***)csmp)[(0x170/4)]-1;
#endif
			WSHDR *ws_fn=((WSHDR **)csmp)[0x2C/4];
			//[1] - title
			//[2] - artist
			//[3] - album
			//[6] - total time
			//[7] - track number
			char path[128];
			static int def_cov;	//состояние, загружена дефолтная ли обложка
			static char album_save[128];

			void SetCover(void)
			{
				FreeIMGHDR(cover);
				HObj obj = CreateHObjFromImgFile(path);
				if (obj)
				{
					Obs_SetScaling(obj, 1);
					Obs_SetOutputImageSize(obj, cfg_rc_cover.x2 - cfg_rc_cover.x, cfg_rc_cover.y2 - cfg_rc_cover.y);
					Obs_Start(obj);
					cover = HObj2IMGHDR(obj);
				}
				Obs_DestroyObject(obj);
			}


			if (GetPlayStatus())
			{
				wstrcpy(ws_title, wws[1]);
				wstrcpy(ws_artist, wws[2]);
				wstrcpy(ws_fname, ws_fn);

				if (wws[3] && wws[3]->wsbody[0])
				{
					char album_utf8[128];

					wstrcpy(ws_album, wws[3]);
					ws_2str(ws_album, album_utf8, (wws[3]->wsbody[0] + 1) * 2);

					if (strcmp(album_save, album_utf8))
					{
						FSTATS fs;
						unsigned int err;

						strcpy(album_save, album_utf8);
						if (GetCoverPath(path, album_utf8) == -1)
							goto DEFAULT;
						else
						{
							def_cov = 0;
							SetCover();
						}
					}
				}
				else
				{
					album_save[0]='\0';
					DEFAULT:
						if (!def_cov)
						{
							def_cov = 1;
							sprintf(path, "%s%s", img_dir, "mptab\\cover_default.png");
							SetCover();
						}
				}
			}
		}
		else if (player_csm_id = GetCSMIDFromAddr(cfg_mp_csm_adr)) SetSongInfo();
	}
	else if (player_csm_id = GetCSMIDFromAddr(cfg_mp_csm_adr)) SetSongInfo();
}

void DrawCover(int is_on_redraw)
{
	if (!is_on_redraw)
		DrawSeparateBG(cfg_rc_cover.x - 7, cfg_rc_cover.y - 7, cfg_rc_cover.x - 7 + cover_bg->w, cfg_rc_cover.y -7 + cover_bg->h);
	DrawIMGHDR(cover_bg, cfg_rc_cover.x - 7, cfg_rc_cover.y - 7, 0, 0, 0, 0);
	DrawIMGHDR(cover, cfg_rc_cover.x, cfg_rc_cover.y, 0, 0, 0, 0);
}

void DrawSongInfo(int is_on_redraw)
{
	unsigned int font = GetFont(fontMain1);

	if (!is_on_redraw)
	{
		DrawSeparateBG(0, cfg_y_artist, ScreenW(), cfg_y_artist + GetFontYSIZE(font)); //album && fname background
		DrawSeparateBG(0, cfg_y_title, ScreenW(), cfg_y_title + GetFontYSIZE(font));
		DrawSeparateBG(0, cfg_y_fname, ScreenW(), cfg_y_fname + GetFontYSIZE(font));
	}
	if (ws_title->wsbody[0] && ws_artist->wsbody[0])
	{
		DrawString(ws_title, 0, cfg_y_title, ScreenW(), cfg_y_title + GetFontYSIZE(font), font , TEXT_ALIGNMIDDLE,
					GetColor(colorMain), GetPaletteAdrByColorIndex(23));
		DrawString(ws_artist, 0, cfg_y_artist, ScreenW(), cfg_y_artist + GetFontYSIZE(font), font , TEXT_ALIGNMIDDLE,
					GetColor(colorMain), GetPaletteAdrByColorIndex(23));
	}
	else
	{
		DrawString(ws_fname, 0, cfg_y_fname, ScreenW(), cfg_y_fname + GetFontYSIZE(font), font , TEXT_ALIGNMIDDLE,
					GetColor(colorMain), GetPaletteAdrByColorIndex(23));
					}
}

void DrawButtons(int is_on_redraw)
{
	if (!is_on_redraw)
	{
		DrawSeparateBG(cfg_x_prev, cfg_y_prev, cfg_x_prev + icons[iconPrev]->w, cfg_y_prev + icons[iconPrev]->h);
		DrawSeparateBG(cfg_x_pp, cfg_y_pp, cfg_x_pp + icons[iconPause]->w, cfg_y_pp + icons[iconPause]->h);
		DrawSeparateBG(cfg_x_next, cfg_y_next, cfg_x_next + icons[iconNext]->w, cfg_y_next + icons[iconNext]->h);
	}

	//неактивные кнопки
	DrawIMGHDR(icons[iconPrev], cfg_x_prev, cfg_y_prev, 0, 0, 0, 0);
	if (play_status == MP_PLAY)
		DrawIMGHDR(icons[iconPause], cfg_x_pp, cfg_y_pp, 0, 0, 0, 0);
	else
		DrawIMGHDR(icons[iconPlay], cfg_x_pp, cfg_y_pp, 0, 0, 0, 0);
	DrawIMGHDR(icons[iconNext], cfg_x_next, cfg_y_next, 0, 0, 0, 0);
	//активные кнопки
	switch (cur_pos)
	{
		case 0:
			DrawIMGHDR(icons[iconPrev_active], cfg_x_prev, cfg_y_prev, 0, 0, 0, 0);
		break;
		case 1:
			if (play_status == MP_PLAY)
				DrawIMGHDR(icons[iconPause_active], cfg_x_pp, cfg_y_pp, 0, 0, 0, 0);
			else
				DrawIMGHDR(icons[iconPlay_active], cfg_x_pp, cfg_y_pp, 0, 0, 0, 0);
		break;
		case 2:
			DrawIMGHDR(icons[iconNext_active], cfg_x_next, cfg_y_next, 0, 0, 0, 0);
		break;
	}
}

void DrawVol(int is_on_redraw, int key, int mode)
{
	IMGHDR *img1 = icons[iconVol_up];
	IMGHDR *img2 = icons[iconVol_down];
	wsprintf(ws, "%02d", *RamVolumeStep());
	unsigned int font = GetFont(fontMain1);
	if (!is_on_redraw)
	{
		DrawSeparateBG(cfg_x_volume, cfg_y_volume, cfg_x_volume + GetWsWidth(ws, font), cfg_y_volume + GetFontYSIZE(font));
		DrawSeparateBG(cfg_x_vol_up, cfg_y_vol_up, cfg_x_vol_up + icons[iconVol_up]->w, cfg_y_vol_up + icons[iconVol_up]->h);
		DrawSeparateBG(cfg_x_vol_down, cfg_y_vol_down, cfg_x_vol_down + icons[iconVol_down]->w, cfg_y_vol_down + icons[iconVol_down]->h);
		if (key == VOL_UP_BUTTON)
			img1 = (mode != KEY_UP) ? icons[iconVol_up_active] : icons[iconVol_up];
		else if (key == VOL_DOWN_BUTTON)
			img2 = (mode != KEY_UP) ? icons[iconVol_down_active] : icons[iconVol_down];
	}
	DrawIMGHDR(img1, cfg_x_vol_up, cfg_y_vol_up, 0, 0, 0, 0);
	DrawIMGHDR(img2, cfg_x_vol_down, cfg_y_vol_down, 0, 0, 0, 0);
	DrawString(ws, cfg_x_volume, cfg_y_volume, cfg_x_volume + GetWsWidth(ws, font), cfg_y_volume + GetFontYSIZE(font), font, TEXT_ALIGNLEFT,
				GetColor(colorMain), GetPaletteAdrByColorIndex(23));
}

void OnRedraw(void)
{
	SetSongInfo();

	if (play_status == 5)
		play_status = 0;
	else
		play_status = GetPlayStatus();

	DrawText(ws, lgp[lgpHeader], GetFont(fontHeader1), TEXT_ALIGNMIDDLE, GetColor(colorHeader), TEXT_TYPE_HEADER);

	if (play_status)
	{
		DrawCover(1);
		DrawSongInfo(1);
		DrawVol(1, 0, 0);
		DrawButtons(1);
		DrawText(ws, lgp[lgpSoftkeyLeft_turn], GetFont(fontSoftkeys), TEXT_ALIGNLEFT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
		DrawText(ws, lgp[lgpSoftkeyRight_close], GetFont(fontSoftkeys), TEXT_ALIGNRIGHT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	}
	else
	{
		unsigned int font = GetFont(fontMain1);
		wsprintf(ws, "%t", lgp[lgpMain]);
		DrawString(ws, 0, cfg_coord_h_mid - GetFontYSIZE(font)/2, ScreenW(), ScreenH(), font, TEXT_ALIGNMIDDLE,
					GetColor(colorMain), GetPaletteAdrByColorIndex(23));
		DrawText(ws, lgp[lgpSoftkeyLeft_open], GetFont(fontSoftkeys), TEXT_ALIGNLEFT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	}
}

void StopUpdate()
{
	DelTimer(&tmr);
}

void AutoUpdate(void)
{
	if (IsGuiOnTop(shell_gui_id))
	{
		SetSongInfo();
		if (play_status)
		{
			DrawCover(0);
			DrawSongInfo(0);
			GBS_StartTimerProc(&tmr, TMR_6_SEC / 6 * cfg_update_time, (void*)AutoUpdate);
		}
		else
			StopUpdate();
	}
	else
	{
		STOP_UPDATE:
			StopUpdate();
	}
}

void OnFocus(void)
{
	SetSongInfo();
	play_status = GetPlayStatus();
	if (play_status)
		GBS_StartTimerProc(&tmr, TMR_6_SEC / 6 * cfg_update_time, (void*)AutoUpdate);
}

void OnKey(int key, int type)
{
	if (play_status)
	{
		if (type == KEY_DOWN)
		{
			switch (key)
			{
				case UP_BUTTON:
					keyblock_id = plugin_id;
					if (cur_pos != 0)
						cur_pos -= 1;
					else
						cur_pos = 2;
					DrawButtons(0);
				break;
				case DOWN_BUTTON:
					keyblock_id = plugin_id;
					if (cur_pos != 2)
						cur_pos += 1;
					else
						cur_pos = 0;
					DrawButtons(0);
				break;
				case ENTER_BUTTON:
					if (cur_pos == 0)
						MPlayer_Prev();
					else if (cur_pos == 1)
					{
						if (play_status == MP_PLAY)
						{
							MPlayer_Pause();
							play_status = MP_PAUSE;
						}
						else
						{
							MPlayer_Start();
							play_status = MP_PLAY;
						}
					}
					else
						MPlayer_Next();
				DrawButtons(0);
				break;
				case VOL_UP_BUTTON:
					DrawVol(0, key, type);
				break;
				case VOL_DOWN_BUTTON:
					DrawVol(0, key, type);
				break;
				case LEFT_SOFT:
					goto L_SOFT;
				case RIGHT_SOFT:
					CloseCSM(player_csm_id);
					play_status = 5;
					DirectRedrawGUI_ID(shell_gui_id);
				break;
			}
		}
		else if (type == LONG_PRESS)
		{
			switch (key)
			{
				case VOL_UP_BUTTON:
					DrawVol(0, key, type);
				break;
				case VOL_DOWN_BUTTON:
					DrawVol(0, key, type);
				break;
			}
		}
		else if (type == KEY_UP)
		{
			switch (key)
			{
				case VOL_UP_BUTTON:
					DrawVol(0, key, type);
				break;
				case VOL_DOWN_BUTTON:
					DrawVol(0, key, type);
				break;
				case UP_BUTTON: case DOWN_BUTTON:
					keyblock_id = 0;
				break;
			}
		}
	}
	else if (type == KEY_DOWN)
	{
		switch (key)
		{
			L_SOFT:
				case LEFT_SOFT:
					ExecShortcut("MEDIA_PLAYER");
				break;
		}
	}
}

void Destroy(void)
{
	StopUpdate();
	
	if (lgp)
		FreeLang(&lgp);
	if (ws)
	{
		FreeWS(ws_title);
		FreeWS(ws_artist);
		FreeWS(ws_album);
		FreeWS(ws_fname);
		FreeWS(ws);
	}
	
	for (unsigned int i = 0; i < iconsTotal; i++)
		FreeIMGHDR(icons[i]);
	
	FreeIMGHDR(cover);
	FreeIMGHDR(cover_bg);
}

void OnMessage(CSM_RAM *data, GBS_MSG *msg)
{
	if(msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp(successed_config_filename, (char *)msg->data0) == 0)
		{
			InitConfig(successed_config_filename);
			*desk_id_ptr         = cfg_desk_id;
		}
	}
}

int main(PLUGIN_S4T *plg)
{
	//чтение конфига
	char path[128];
	sprintf(path, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(path);

	plg->OnRedraw     = (void*)OnRedraw;
	plg->OnFocus      = (void*)OnFocus;
	plg->OnKey        = (void(*)(unsigned int, unsigned int))OnKey;
	plg->OnMessage    = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;

	plg->Destroy = (void*)Destroy;

	plg->StartUpdate = (void*)AutoUpdate;
	plg->StopUpdate  = (void*)StopUpdate;


	sprintf(path, "%s%s", lang_dir, "mptab.txt");
	if (InitLang(path, &lgp) == -1) return -1;
	
	FSTATS fs;
	unsigned int err;
	char *names[iconsTotal] = {"next", "next-active", "pause", "pause-active", "play", "play-active", "prev", "prev-active", "vol_up",
		"vol_up-active", "vol_down", "vol_down-active", "cover_bg"};
	for (unsigned int i = 0; i < iconsTotal; i++)
	{
		sprintf(path, "%s%s%s%s", img_dir, "mptab\\", names[i], ".png");
		if (GetFileStats(path, &fs, &err) == -1) return -1;
		icons[i] = CreateIMGHDRFromPngFile(path, 0);
	}
	
	ws_title    = AllocWS(128);
	ws_artist   = AllocWS(128);
	ws_album    = AllocWS(128);
	ws_fname    = AllocWS(128);
	ws          = AllocWS(128);
	
	plg->desk_id = cfg_desk_id;
	desk_id_ptr  = &plg->desk_id;
	plugin_id    = plg->id;
	return 0;
}
