#include <swilib.h>
#include "../../../libsiemens/other.h"
#include "../../../libsiemens/swihook.h"
#include "../../../libsiemens/cfg.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/other.h"
#include "main.h"
#include "conf_loader.h"
#include "config_data.h"
#include "gui.h"

#define  DLONG  tmr.param6

GBSTMR tmr;
unsigned int *desk_id_ptr;
unsigned int *swi_addr;
unsigned int plugin_id;
unsigned int autolock_sec;

void DLock(void)
{
	DLOCK = 0;
	keyblock_id = 0;
	DelTimer(&tmr);
	if (!DLONG)
		GBS_SendMessage(MMI_CEPID, KEY_UP, cfg_key);
	else
	{
		DLONG = 0;
		GBS_SendMessage(MMI_CEPID, LONG_PRESS, cfg_key);
	}
}

void KeyboardLock(void)
{
	KbdLock();
}

void AutoLock(void)
{
	SettingsAE_Read(&autolock_sec, AE_APIDC_SETUP, "SecurityUI", "AutoKeyLock");
	if (autolock_sec)
	{
		if (IsGuiOnTop(shell_gui_id))
			GBS_StartTimerProc(&tmr, TMR_6_SEC / 6 * autolock_sec, (void*)KeyboardLock);
	}
}

void OnKey(unsigned int key, unsigned int type)
{
	if (key == cfg_key)
	{
		switch (cfg_type_block)
		{
			case CFG_KEY_DOWN:
				if (type == KEY_DOWN)
				{
					keyblock_id = plugin_id;
					KbdLock();
				}
			break;
			case CFG_LONG_PRESS:
				if (type == LONG_PRESS)
				{
					keyblock_id = plugin_id;
					KbdLock();
				}
			break;
			case CFG_DOUBLE_CLICK:
				if (type == KEY_DOWN)
				{
					if (!DLOCK)
					{
						DLOCK = 1;
						keyblock_id = plugin_id;
						GBS_StartTimerProc(&tmr, cfg_wait_time, (void*)DLock);
					}
					else if (DLOCK == 1)
					{
						KbdLock();
					}
				}
				else if (type == LONG_PRESS)
					DLONG = 1;
			break;
		}
	}
	//автоблокировка
	if (autolock_sec)
	{
		if (type == KEY_DOWN)
			DelTimer(&tmr);
		else if (type == KEY_UP)
			AutoLock();
	}
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

void OnUnFocus(void)
{
	DelTimer(&tmr);
}

void Destroy(void)
{
	DelTimer(&tmr);
	if (swi_addr)
		DestroySWIHook(SWI_KBDLOCK, swi_addr); 
}

int main(PLUGIN_S4T *plg)
{
	//чтение конфига
	char bcfg[128];
	sprintf(bcfg, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(bcfg);

	plg->OnKey     = (void(*)(unsigned int, unsigned int))OnKey;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;
	plg->OnFocus   = (void*)AutoLock;
	plg->OnUnFocus = (void*)OnUnFocus;
	plg->Destroy   = (void*)Destroy;
	
	swi_addr = SetSWIHook(SWI_KBDLOCK, (void*)CreateSSGUI);
	if (swi_addr == NULL) return -1;

	plg->desk_id = cfg_desk_id;
	desk_id_ptr  = &plg->desk_id;
	plugin_id    = plg->id;
	return 0;
}
