#include <swilib.h>
#include "../../../libsiemens/files.h"
#include "../../../libsiemens/other.h"
#include "../../../libsiemens/graphics.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/other.h"
#include "../../../libshell/graphics.h"
#include "conf_loader.h"
#include "config_data.h"

#define DISPLAY_ICONS 0x05

#define LEFT  LEFT_BUTTON
#define RIGHT RIGHT_BUTTON

#define ANIMATION_STOP  tmr.param4
#define ANIMATION_NAVI  tmr.param5
#define ANIMATION_COUNT tmr.param6

GBSTMR tmr;
IMGHDR *panel;
IMGHDR **icons_unsel, **icons_sel;

unsigned int x_start;
unsigned int anim_lenght;
unsigned int plugin_id;

void DrawPanel(void)
{
	unsigned int x;
	unsigned int y;
	unsigned int y2;
	//фон
	y  = ScreenH() - img[imgBottom]->h - panel->h;
	y2 = ScreenH() - img[imgBottom]->h - 1;
	DrawSeparateBG(0, y, ScreenW(), y2);
	//панель
	DrawIMGHDR(panel, 0, y, 0, 0, 0, 0);
	//иконки
	y = ScreenH() - img[imgBottom]->h - panel->h + (panel->h - icons_unsel[0]->h) / 2;
	
	unsigned int id_max;
	unsigned int id_start;
	if (desk_total <= 5)
	{
		id_max = desk_total;
		id_start = 0;
	}
	else
	{
		if (desk_total - cur_desk_id >= 4)
			id_start = (cur_desk_id == 1) ? cur_desk_id - 1 : cur_desk_id - 2;
		else
			id_start = desk_total - 6;
		if (cur_desk_id > desk_total - 4) id_max = 6;
		else
		{
			if (cur_desk_id < desk_total - 4) id_max = 7;
			else if (cur_desk_id < desk_total - 3) id_max = 6;
			else id_max = 5;
		}
	}
		
	for (unsigned int i = 0; i < id_max; i++)
	{
		x = x_start + anim_lenght * i;
		if (id_start + 1 != cur_desk_id)
			DrawIMGHDR(icons_unsel[id_start++], x, y, 0, 0, 0, 0);
		else
			DrawIMGHDR(icons_sel[id_start++], x, y, 0, 0, 0, 0);
	}
}

void OnRedraw(void)
{
	DrawPanel();
}

void MFunc(void)
{
	for (int i = 0; plg[i] != NULL; i++)
	{
		if (!IsUsePlg(plg[i]))
		{
			if (plg[i]->OnUnFocus) plg[i]->OnUnFocus();
		}
		else
		{
			if (plg[i]->OnFocus) plg[i]->OnFocus();
		}
	}
}

void Animation(void)
{
	void InitData(void)
	{
	if (cur_desk_id > 1 && cur_desk_id < desk_total - 3)
		x_start = anim_lenght * -1;
	else
		x_start = 0;
	}
	if (ANIMATION_COUNT % ((anim_lenght + 1)/2) == 0)
	{
		UNFOCUS:
			switch (ANIMATION_NAVI)
			{
				case LEFT:
					cur_desk_id--;
					InitData();
					if (cur_desk_id == 1) goto EXIT;
				break;
				case RIGHT:
					cur_desk_id++;
					InitData();
					if (cur_desk_id == desk_total - 4) goto EXIT;
				break;
			}
			if (ANIMATION_STOP)
			{
				EXIT:
					ANIMATION_STOP  = 0;
					ANIMATION_COUNT = 1;
					ANIMATION_NAVI  = 0;
					DelTimer(&tmr);
					keyblock_id = 0;
					if (IsGuiOnTop(shell_gui_id))
					{
						MFunc();
						DirectRedrawGUI_ID(shell_gui_id);
					}
					return;
			}
	}
	if (ANIMATION_NAVI == RIGHT)
		x_start -= 2;
	else
		x_start += 2;
	ANIMATION_COUNT++;
	//завершаем наш цикл
	if (!IsGuiOnTop(shell_gui_id)) goto UNFOCUS;
	DrawPanel();
	GBS_StartTimerProc(&tmr, 2, (void*)Animation);
}

void OnKey(unsigned int key, unsigned int type)
{
	switch (type)
	{
		case KEY_DOWN:
			if (key == LEFT_BUTTON || key == RIGHT_BUTTON)
			{
				keyblock_id = plugin_id;
				GBS_SendMessage(MMI_CEPID, LONG_PRESS, key);
			}
		break;
		case LONG_PRESS:
			if (key == LEFT_BUTTON)
			{
				keyblock_id = plugin_id;
				if (cur_desk_id != 1)
				{
					if (cur_desk_id <= desk_total - 4)
					{
						if (!ANIMATION_NAVI || ANIMATION_NAVI == (int)key)
						{
							ANIMATION_NAVI = key;
							Animation();
						}
					}
					else if (ANIMATION_COUNT == 1)
					{
						cur_desk_id--;
						MFunc();
						DirectRedrawGUI_ID(shell_gui_id);
					}
				}
			}
			else if (key == RIGHT_BUTTON)
			{
				keyblock_id = plugin_id;
				if ((int)cur_desk_id > (int)(desk_total - 5))
				{
					if (cur_desk_id != desk_total && ANIMATION_COUNT == 1)
					{
						cur_desk_id++;
						MFunc();
						DirectRedrawGUI_ID(shell_gui_id);
					}
				}
				else
				{
					if (!ANIMATION_NAVI || ANIMATION_NAVI == (int)key)
					{
						ANIMATION_NAVI = key;
						Animation();
					}
				}
			}
		break;
		case KEY_UP:
			if (key == LEFT_BUTTON || key == RIGHT_BUTTON)
			{
				ANIMATION_STOP = 1;
				if (ANIMATION_COUNT == 1) keyblock_id = 0;
			}
		break;
	}
}

void OnMessage(CSM_RAM *data, GBS_MSG *msg)
{
	if (msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp(successed_config_filename, (char*)msg->data0) == 0)
		{
			InitConfig(successed_config_filename);
			anim_lenght     = icons_unsel[0]->w + cfg_icons_space;
		}
	}
}

void Destroy(void)
{
	FreeIMGHDR(panel);
	if (icons_unsel)
	{
		for (unsigned int i = 0 ; i < desk_total; i++)
			FreeIMGHDR(icons_unsel[i]);
		mfree(icons_unsel);
	}
	if (icons_sel)
	{
		for (unsigned int i = 0; i < desk_total; i++)
			FreeIMGHDR(icons_sel[i]);
		mfree(icons_sel);
	}
}

int main(PLUGIN_S4T *plg)
{
	//чтение конфига
	char path[128];
	sprintf(path, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(path);
	
	plg->OnRedraw  = (void*)OnRedraw;
	plg->OnKey     = (void(*)(unsigned int, unsigned int))OnKey;
	plg->Destroy   = (void*)Destroy;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;
	
	//загружаем графику
	FSTATS fs;
	unsigned int err;
	sprintf(path, "%s%s", img_dir, "deskswitcher\\panel.png");
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	panel = CreateIMGHDRFromPngFile(path, 0);
	
	DIR_ENTRY **de;
	sprintf(path, "%s%s", img_dir, "deskswitcher\\icons\\");
	unsigned int total = FindFiles(&de, path, "*.png"); //считаем графику

	if (total == 0) return -1;
	
	DE_Sort1(&de);
	
	icons_unsel = malloc(sizeof(IMGHDR*) * (total / 2));
	icons_sel = malloc(sizeof(IMGHDR*) * (total / 2));
	while(desk_total < (total / 2))
	{
		sprintf(path, "%s%s%s", img_dir, "deskswitcher\\icons\\", de[desk_total * 2 + 1]->file_name);
		icons_unsel[desk_total] = malloc(sizeof(IMGHDR));
		icons_unsel[desk_total] = CreateIMGHDRFromPngFile(path, 0);
		sprintf(path, "%s%s%s", img_dir, "deskswitcher\\icons\\", de[desk_total * 2]->file_name);
		icons_sel[desk_total] = malloc(sizeof(IMGHDR));
		icons_sel[desk_total++] = CreateIMGHDRFromPngFile(path, 0);
	}
	
	DE_Free(&de);
	
	plg->desk_id = 0;
	plugin_id = plg->id;
	
	anim_lenght     = icons_unsel[0]->w + cfg_icons_space;
	ANIMATION_COUNT = 1;
}
