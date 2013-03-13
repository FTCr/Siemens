#include <swilib.h>
#include "../../../libsiemens/lang.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/other.h"
#include "conf_loader.h"
#include "config_data.h"
#include "gui.h"

unsigned int *desk_id_ptr;

void OnKey(unsigned int key, unsigned int type)
{
	switch (type)
	{
		case LONG_PRESS:
			if (key >= '1' && key <= '9')
				CreateDialGUI(key - '\0');
			else if (key == '0')
				CreateDialGUI('+');
			else if (key == '#')
				CreateDialGUI('?');
		break;
		case KEY_UP:
			if (key >= '0' && key <='9')
				CreateDialGUI(key - '\0');
			else if (key == '*')
				CreateDialGUI('*');
			else if (key == '#')
				CreateDialGUI('#');
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
			*desk_id_ptr = cfg_desk_id;
		}
	}
}

void Destroy(void)
{
	if (lgp)
		FreeLang(&lgp);
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
	
	sprintf(path, "%s%s", lang_dir, "dgui.txt");
	if (InitLang(path, &lgp) == -1) return -1;
	
	plg->desk_id     = cfg_desk_id;
	desk_id_ptr = &plg->desk_id;
	return 0;
}
