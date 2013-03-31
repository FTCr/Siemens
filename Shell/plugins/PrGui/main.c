#include <swilib.h>
#include "../../../libsiemens/lang.h"
#include "../../../libsiemens/graphics.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/other.h"
#include "main.h"
#include "gui.h"
#include "conf_loader.h"
#include "config_data.h"

IMGHDR *icon[1];

char **lgp;

unsigned int plugin_id;
unsigned int *desk_id_ptr;

void OnKey(unsigned int key, unsigned int type)
{
	if (type == KEY_DOWN)
	{
		if (cfg_key == key)
		{
			keyblock_id = plugin_id;
			CreatePrGUI();
		}
	}
	else if (type == KEY_UP)
	{
		if (cfg_key == key)
		{
			keyblock_id = 0;
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
	if (lgp)
		FreeLang(&lgp);
	FreeIMGHDR(icon[0]);
}

int main(PLUGIN_S4T *plg)
{
	//чтение конфига
	char path[128];
	sprintf(path, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(path);
	
	plg->OnKey     = (void(*)(unsigned int, unsigned int))OnKey;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;
	plg->Destroy   = (void*)Destroy;
	plg->OnClose   = (void*)OnClose;
	
	sprintf(path, "%s%s", lang_dir, "prgui.txt");
	if (InitLang(path, &lgp) == -1) return -1;
	
	sprintf(path, "%s%s%s", img_dir, "prgui\\", "icon.png");
	icon[0] = CreateIMGHDRFromPngFile(path, 0);
	
	plg->desk_id = cfg_desk_id;
	desk_id_ptr  = &plg->desk_id;
	plugin_id    = plg->id;
	
	return 0;
}
