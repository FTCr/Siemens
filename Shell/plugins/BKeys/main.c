#include <swilib.h>
#include "../../../libsiemens/other.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/other.h"
#include "conf_loader.h"
#include "config_data.h"

unsigned int *desk_id_ptr;
unsigned int plugin_id;

void OnKey(unsigned int key, unsigned int type)
{
	void Execute(unsigned int cfg_use, unsigned int cfg_style, unsigned int cfg_action, const char *cfg_execute)
	{
		if (cfg_use)
		{
			switch (type)
			{
				case KEY_DOWN:
					if (cfg_style == 0)
						goto EXEC;
				break;
				case LONG_PRESS:
					if (cfg_style == 1)
					{
						EXEC:
							keyblock_id = plugin_id;
							if (cfg_action == 0)
								ExecFile(cfg_execute);
							else
								ExecShortcut(cfg_execute);
					}
				break;
			}
		}
	}
	
	if (type == KEY_DOWN || type == LONG_PRESS)
	{
		switch (key)
		{
			case LEFT_BUTTON:
				Execute(cfg_use_lb, cfg_style_lb, cfg_action_lb, cfg_execute_lb);
			break;
			case RIGHT_BUTTON:
				Execute(cfg_use_rb, cfg_style_rb, cfg_action_rb, cfg_execute_rb);
			break;
			case UP_BUTTON:
				Execute(cfg_use_ub, cfg_style_ub, cfg_action_ub, cfg_execute_ub);
			break;
			case DOWN_BUTTON:
				Execute(cfg_use_db, cfg_style_db, cfg_action_db, cfg_execute_db);
			break;
			case GREEN_BUTTON:
				Execute(cfg_use_gb, cfg_style_gb, cfg_action_gb, cfg_execute_gb);
			break;
			case ENTER_BUTTON:
				Execute(cfg_use_eb, cfg_style_eb, cfg_action_eb, cfg_execute_eb);
			case '1':
				Execute(cfg_use_1, cfg_style_1, cfg_action_1, cfg_execute_1);
			break;
			case '2':
				Execute(cfg_use_2, cfg_style_2, cfg_action_2, cfg_execute_2);
			break;
			case '3':
				Execute(cfg_use_3, cfg_style_3, cfg_action_3, cfg_execute_3);
			break;
			case '4':
				Execute(cfg_use_4, cfg_style_4, cfg_action_4, cfg_execute_4);
			break;
			case '5':
				Execute(cfg_use_5, cfg_style_5, cfg_action_5, cfg_execute_5);
			break;
			case '6':
				Execute(cfg_use_6, cfg_style_6, cfg_action_6, cfg_execute_6);
			break;
			case '7':
				Execute(cfg_use_7, cfg_style_7, cfg_action_7, cfg_execute_7);
			break;
			case '8':
				Execute(cfg_use_8, cfg_style_8, cfg_action_8, cfg_execute_8);
			break;
			case '9':
				Execute(cfg_use_9, cfg_style_9, cfg_action_9, cfg_execute_9);
			break;
			case '*':
				Execute(cfg_use_star, cfg_style_star, cfg_action_star, cfg_execute_star);
			break;
			case '0':
				Execute(cfg_use_0, cfg_style_0, cfg_action_0, cfg_execute_0);
			break;
			case '#':
				Execute(cfg_use_lat, cfg_style_lat, cfg_action_lat, cfg_execute_lat);
			break;
		}
	}
}

void OnUnFocus(void)
{
	keyblock_id = 0;
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
	char path[128];
	sprintf(path, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(path);

	plg->OnKey     = (void(*)(unsigned int, unsigned int))OnKey;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;
	plg->OnUnFocus = (void*)OnUnFocus;
	
	plg->desk_id = cfg_desk_id;
	desk_id_ptr  = &plg->desk_id;
	plugin_id    = plg->id;
	
	return 0;
}
