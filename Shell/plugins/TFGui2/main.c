#include <swilib.h>
#include "../../../libsiemens/graphics.h"
#include "../../../libsiemens/lang.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/other.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/skin_utils.h"
#include "main.h"
#include "gui.h"
#include "conf_loader.h"
#include "config_data.h"

IMGHDR *icons[MENU_ITEMS];
char *strings1[MENU_ITEMS], *strings2[MENU_ITEMS];

unsigned int *desk_id_ptr;

char **lgp;
WSHDR *ws;

void OnRedraw(void)
{
	DrawText(ws, lgp[lgpMainSoft], GetFont(fontSoftkeys), (cfg_key == 0) ? TEXT_ALIGNLEFT : TEXT_ALIGNRIGHT, GetColor(colorSoftkeys),
		TEXT_TYPE_SOFTKEY);
}

void OnKey(unsigned int key, unsigned int type)
{
	if (type == KEY_DOWN)
	{
		switch (key)
		{
			case LEFT_SOFT:
				if (cfg_key == 0) CreateTFGUI2();
			break;
			case RIGHT_SOFT:
				if (cfg_key == 1) CreateTFGUI2();
			break;
		}
	}
}

void OnMessage(CSM_RAM *data, GBS_MSG *msg)
{
	if (msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp(successed_config_filename, (char*)msg->data0) == 0)
		{
			InitConfig(successed_config_filename);
			*desk_id_ptr = cfg_desk_id;
		}
	}
}

void OnClose(void)
{
	if (gui_id)
		GeneralFunc_flag1(gui_id, 0);
}

void Destroy(void)
{
	if (ws)
		FreeWS(ws);
	if (lgp)
		FreeLang(&lgp);
	unsigned int i = 0;
	while (i < MENU_ITEMS)
		FreeIMGHDR(icons[i++]);
}

int main(PLUGIN_S4T *plg)
{
	//чтение конфига
	char path[128];
	sprintf(path, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(path);
	
	plg->OnRedraw  = (void*)OnRedraw;
	plg->OnKey     = (void(*)(unsigned int, unsigned int))OnKey;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;
	plg->OnClose   = (void*)OnClose;
	plg->Destroy   = (void*)Destroy;
	
	sprintf(path, "%s%s", lang_dir, "tfgui2.txt");
	if (InitLang(path, &lgp) == -1) return -1;
	
	FSTATS fs;
	unsigned int err;
	unsigned int i      = 0;
	unsigned int lgp_id = 4;
	while(i < MENU_ITEMS)
	{
		strings1[i] = lgp[lgp_id];
		strings2[i] = lgp[lgp_id + 1];
		sprintf(path, "%s%s%d%s", img_dir, "tfgui2\\", i + 1, ".png");
		icons[i++] = CreateIMGHDRFromPngFile(path, 0);
		lgp_id += 2;
	}
	
	ws = AllocWS(128);
	
	plg->desk_id     = cfg_desk_id;
	desk_id_ptr = &plg->desk_id;
	
	return 0;
}
