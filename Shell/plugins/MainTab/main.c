#include <swilib.h>
#include "../../../libshell/plugins.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/skin_utils.h"
#include "../../../libshell/other.h"
#include "../../../libsiemens/other.h"
#include "../../../libsiemens/lang.h"
#include "conf_loader.h"
#include "config_data.h"

char **lgp;

WSHDR *ws;

unsigned int *desk_id_ptr;

enum
{
	lgpSoftkeyLeft,
	lgpSoftkeyRight,
};

void OnRedraw(void)
{
	if (cfg_use_ls) DrawText(ws, lgp[lgpSoftkeyLeft], GetFont(fontSoftkeys), TEXT_ALIGNLEFT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	if (cfg_use_rs) DrawText(ws, lgp[lgpSoftkeyRight], GetFont(fontSoftkeys), TEXT_ALIGNRIGHT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
}

void OnKey(int key, int type)
{
	if (type == KEY_DOWN)
	{
		switch (key)
		{
			case LEFT_SOFT:
				if (cfg_use_ls) ExecShortcut(cfg_ls_func);
			break;
			case RIGHT_SOFT:
				if (cfg_use_rs) ExecShortcut(cfg_rs_func);
			break;
			case ENTER_BUTTON:
				if (cfg_use_enter) ExecShortcut(cfg_enter_func);
			break;
			case GREEN_BUTTON:
				ExecShortcut("REC_DIALED_LIST");
			break;
		}
	}
}

void Destroy(void)
{
	FreeLang(&lgp);
	FreeWS(ws);
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

void main(PLUGIN_S4T *plg)
{
	char path[128];
	sprintf(path, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(path);

	plg->OnRedraw  = (void*)OnRedraw;
	plg->OnKey     = (void(*)(unsigned int, unsigned int))OnKey;
	plg->Destroy   = (void*)Destroy;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;

	plg->desk_id = cfg_desk_id;
	desk_id_ptr  = &plg->desk_id;

	sprintf(path, "%s%s", lang_dir, "maintab.txt");
	InitLang(path, &lgp);

	ws = AllocWS(64);
}
