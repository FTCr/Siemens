#include <swilib.h>
#include "../../../libsiemens/lang.h"
#include "../../../libsiemens/files.h"
#include "../../../libsiemens/graphics.h"
#include "../../../libsiemens/other.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/other.h"
#include "../../../libshell/menu.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/skin_utils.h"
#include "../../../libshell/config_coord.h"
#include "conf_loader.h"
#include "config_data.h"

DIR_ENTRY **de;

int total_items;

MENU *menu;
WSHDR **ws1, **ws2;
IMGHDR *icon[1];

char **lgp;
enum
{
	lgpHeader,
	lgpSoftkeyLeft,
	lgpSoftkeyRight,
	lgpMain
};


WSHDR *ws;

unsigned int *desk_id_ptr;
unsigned int plugin_id;

void OnRedraw(void)
{
	DrawText(ws, lgp[lgpHeader], GetFont(fontHeader1), TEXT_ALIGNMIDDLE, GetColor(colorHeader), TEXT_TYPE_HEADER);
	if (total_items)
	{
		DrawText(ws, lgp[lgpSoftkeyLeft], GetFont(fontSoftkeys), TEXT_ALIGNLEFT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
		DrawSMenu(menu);
	}
	else
	{
		wsprintf(ws, "%t", lgp[lgpMain]);
		DrawString(ws, 0, cfg_coord_h_mid, ScreenW(), ScreenH(), GetFont(fontMain1), TEXT_ALIGNMIDDLE, GetColor(colorMain),
			GetPaletteAdrByColorIndex(23));
	}
	DrawText(ws, lgp[lgpSoftkeyRight], GetFont(fontSoftkeys), TEXT_ALIGNRIGHT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
}

void ClearMenuData(void)
{
	if (menu)
	{
		DestroySMenu(menu);
		menu = NULL;
	}
}

void CreateBookmarsMenu(void)
{
	int total = FindFiles(&de, cfg_path_bm_bookmark_dir, "*.url");
	
	if (total) DE_Sort1(&de);
	
	if (total == total_items) return;
	
	ClearMenuData();
	
	total_items = total;
	
	FSTATS fs;
	unsigned int err;
	int len;
	char path[128], *buffer;
	for (int i = 0; i < total_items; i++)
	{
		//имена файлов без расширения
		len = strlen(de[i]->file_name);
		ws1 = realloc(ws1, sizeof(WSHDR*) * (i + 1));
		ws1[i] = AllocWS(len - 3);
		str_2ws(ws1[i], de[i]->file_name, len - 4);
		
		//читаем адреса загладок
		sprintf(path, "%s%s", cfg_path_bm_bookmark_dir, de[i]->file_name);
		int fp = _open(path, A_ReadOnly + A_BIN, P_READ, &err);
		
		GetFileStats(path, &fs, &err);
		buffer = malloc(fs.size + 1);
		buffer[fs.size] = '\0';
		_read(fp, buffer, fs.size, &err);
		_close(fp, &err);
		
		ws2 = realloc(ws2, sizeof(WSHDR*) * (i + 1));
		ws2[i] = AllocWS(fs.size + 1);
		str_2ws(ws2[i], buffer, fs.size);
		mfree(buffer);
	}
	menu = CreateSMenu(ws1, ws2, icon, MENU_ONE_ICONS, NULL, ICONBAR_H + img[imgHeader]->h + cfg_coord_menu1_off_y,
		cfg_coord_max_menu1_items, total);
}

void OnFocus(void)
{
	CreateBookmarsMenu();
	RedrawIconbar();
}

void OnUnFocus(void)
{
	if (de)
	{
		DE_Free(&de);
		de = NULL;
	}
}

void Destroy(void)
{
	if (de)
		DE_Free(&de);
	ClearMenuData();
	FreeIMGHDR(icon[0]);
	if (ws)
		FreeWS(ws);
	
	int i = 0;
	if (ws1)
	{
		while (i < total_items)
			FreeWS(ws1[i++]);
	}
	if (ws2)
	{
		i = 0;
		while (i < total_items)
			FreeWS(ws2[i++]);
	}
}

void RunBookmark(void)
{
	char path[128];
	sprintf(path, "%s%s", cfg_path_bm_bookmark_dir, de[menu->cur_id]->file_name);
	ExecFile(path);
}

void OnKey(unsigned int key, unsigned int type)
{
	if (type == KEY_DOWN || type == LONG_PRESS)
	{
		if (total_items)
		{
			if (key == UP_BUTTON)
			{
				keyblock_id = plugin_id;
				OnKeySMenu(menu, MENU_UP);
			}
			else if (key == DOWN_BUTTON)
			{
				keyblock_id = plugin_id;
				OnKeySMenu(menu, MENU_DOWN);
			}
			else if (key == ENTER_BUTTON)
			{
				keyblock_id = plugin_id;
				RunBookmark();
			}
			else if (key == LEFT_SOFT)
				RunBookmark();
		}
		if (key == RIGHT_SOFT)
			ExecFile(cfg_path_bm);
	}
	else if (type == KEY_UP)
	{
		if (key == UP_BUTTON || key == DOWN_BUTTON || key == ENTER_BUTTON)
			keyblock_id = 0;
	}
}

void OnMessage(CSM_RAM *data, GBS_MSG *msg)
{
	if(msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp(successed_config_filename, (char *)msg->data0) == 0)
		{
			InitPlgConfig(successed_config_filename);
			*desk_id_ptr         = cfg_desk_id;
		}
	}
}

int main(PLUGIN_S4T *plg)
{
	//чтение конфига
	char path[128];
	sprintf(path, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitPlgConfig(path);
	
	
	plg->Destroy   = (void*)Destroy;
	plg->OnRedraw  = (void*)OnRedraw;
	plg->OnKey     = (void(*)(unsigned int, unsigned int))OnKey;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;
	plg->OnFocus   = (void*)OnFocus;
	plg->OnUnFocus = (void*)OnUnFocus;
	
	//загрузка ленгпака
	sprintf(path, "%s%s", lang_dir, "browsertab.txt");
	if (InitLang(path, &lgp) == -1) return -1;
	
	ws  = AllocWS(128);
	
	plg->desk_id = cfg_desk_id;
	desk_id_ptr  = &plg->desk_id;
	plugin_id    = plg->id;
	
	sprintf(path, "%s%s", img_dir, "url.png");
	icon[0] = CreateIMGHDRFromPngFile(path, 0);
	CreateBookmarsMenu();
	
	return 0;
}
