#include <swilib.h>
#include "../../../libsiemens/lang.h"
#include "../../../libsiemens/graphics.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/other.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/skin_utils.h"
#include "main.h"
#include "gui.h"
#include "conf_loader.h"
#include "config_data.h"

unsigned int *desk_id_ptr;

char **lgp;
WSHDR *ws;
IMGHDR *icons_sel[MAX_ICONS];
IMGHDR *icons_unsel[MAX_ICONS];

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
				if (cfg_key == 0) CreateTFGUI();
			break;
			case RIGHT_SOFT:
				if (cfg_key == 1) CreateTFGUI();
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
	for (int i = 0; i < MAX_ICONS; i++)
	{
		FreeIMGHDR(icons_sel[i]);
		FreeIMGHDR(icons_unsel[i]);
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
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;
	plg->Destroy   = (void*)Destroy;
	plg->OnClose   = (void*)OnClose;
	
	sprintf(path, "%s%s", lang_dir, "tfgui.txt");
	if (InitLang(path, &lgp) == -1) return -1;
	
	FSTATS fs;
	unsigned int err;
	char *fname_sel[MAX_ICONS]   = {"reboot_sel", "shutdown_sel", "exit_sel"};
	char *fname_unsel[MAX_ICONS] = {"reboot_unsel", "shutdown_unsel", "exit_unsel"};
	for (int i = 0; i< MAX_ICONS; i++)
	{
		sprintf(path, "%s%s%s%s", img_dir, "tfgui\\", fname_sel[i], ".png");
		if (GetFileStats(path, &fs, &err) == -1) return -1;
		icons_sel[i] = CreateIMGHDRFromPngFile(path, 0);
		
		sprintf(path, "%s%s%s%s", img_dir, "tfgui\\", fname_unsel[i], ".png");
		if (GetFileStats(path, &fs, &err) == -1) return -1;
		icons_unsel[i] = CreateIMGHDRFromPngFile(path, 0);
	}
	
	ws = AllocWS(128);
	
	plg->desk_id     = cfg_desk_id;
	desk_id_ptr = &plg->desk_id;
	
	return 0;
}
