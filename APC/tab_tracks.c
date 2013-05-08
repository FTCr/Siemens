#include <swilib.h>
#include <libapd.h>
#include <libid3.h>
#include "../libsiemens/strings.h"
#include "../libsiemens/obs.h"
#include "main.h"
#include "options_tab_tracks.h"
#include "rewind.h"

void *tab_tracks_gui;

const int icon_mp3 = ICON_MP3_SMALL;
const int icon_aac = ICON_AAC_SMALL;
const int icon_wav = ICON_WAV_SMALL;

static SOFTKEY_DESC SK_TabTracks[]=
{
	{0x0018, 0x0000, (int)"Options"},
	{0x0001, 0x0000, (int)"Exit"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC + 4} //иконка play
};

static int OnKey_TabTracks(void *data, GUI_MSG *msg)
{
	const int Key   = msg->gbsmsg->submess;
	const int Style = msg->gbsmsg->msg;
	if (Style == KEY_DOWN || Style == LONG_PRESS)
	{
		switch (Key)
		{
			int i = 0;
			case LEFT_SOFT:
				CreateOptionsTabTracks();
			break;
			case ENTER_BUTTON:
				i = GetCurMenuItem(data);
				APlayer_PlayTrackID(i);
			break;
			case GREEN_BUTTON:
				SetCursorToMenuItem(data, APlayer_GetTrack() - 1);
				RefreshGUI();
			break;
			case '1':
				APlayer_Rewind(5);
				CreateRewindGUI(Key);
			break;
			case '3':
				APlayer_Forward(5);
				CreateRewindGUI(Key);
			break;
			case '4':
				APlayer_PrevTrack();
			break;
			case '5':
				APlayer_Toggle();
			break;
			case '6':
				APlayer_NextTrack();
			case '*':
				APlayer_MutedToggle();
			break;
			case '#':
				CutItem(data);
				RefreshGUI();
			break;
			
				
		}
	}
	return 0;
}

static void GHook_TabTracks(void *data, int cmd)
{
	if (cmd == TI_CMD_CREATE)
	{
		SK_TabTracks[0].lgp_id  = (int)lgp[lgpOptions];
		SK_TabTracks[1].lgp_id  = (int)lgp[lgpExit];
		SetCursorToMenuItem(data, APlayer_GetTrack() - 1);
	}
}

static void Handler_TabTracks(void *data, int cur_item, void *user_pointer)
{
	
	void *item = AllocMLMenuItem(data);
	DIR_ENTRY_LIST *ptr = APlayer_GetPtr(cur_item);
	WSHDR *ws1;
	WSHDR *ws2;
	
	void SetFname(void)
	{
		char fname[128];
		ws1 = AllocMenuWS(data, 128);
		ws2 = AllocMenuWS(data, 2);
		if (GetFileNameWithoutExtByPath(fname, ptr->path) != -1)
			str_2ws(ws1, fname, strlen(fname));
		else
			str_2ws(ws1, "Error GetFileNameWithoutExt", 64);
	}
	
	if (ptr)
	{
		ID3 *id3 = ptr->data;
		if (id3)
		{
			#define UTF16_ALIGN_RIGHT (0xE01D)
			int len;
			if (id3->album && id3->genre)
			{
				len = id3->album->wsbody[0] + id3->genre->wsbody[0] + 2;
				ws2 = AllocMenuWS(data, len);
				wsprintf(ws2, "%w%c %w", id3->album, UTF16_ALIGN_RIGHT, id3->genre);
				
			}
			else
			{
				ws2 = AllocMenuWS(data, 2);
			}
			if (id3->artist && id3->title)
			{
				len = id3->artist->wsbody[0] + id3->title->wsbody[0] + 5;
				ws1 = AllocMenuWS(data, len);
				wsprintf(ws1, "%w - %w", id3->artist, id3->title);
			}
			else
			{
				SetFname();
			}
		}
		else
		{
			SetFname();
		}
		int uid = GetExtUidByFileName(ptr->path);
		if (uid == UID_MP3)
			SetMenuItemIconArray(data, item, &icon_mp3);
		else if (uid == UID_AAC)
			SetMenuItemIconArray(data, item, &icon_aac);
		else if (uid == UID_WAV)
			SetMenuItemIconArray(data, item, &icon_wav);
	}
	else
	{
		ws1 = AllocMenuWS(data, 32);
		str_2ws(ws1, "Error DIR_ENTRY_LIST", 32);
		ws2 = AllocMenuWS(data, 2);
	}
	SetMLMenuItemText(data, item, ws1, ws2, cur_item);
}

extern int softkeys[];

static SOFTKEYSTAB skt =
{
	SK_TabTracks, 0
};

ML_MENU_DESC Desc_TabTracks =
{
	8, OnKey_TabTracks, GHook_TabTracks, NULL,
	softkeys,
	&skt,
	0x11,
	Handler_TabTracks,
	NULL,
	NULL,
	0,
	1
};