#include <swilib.h>
#include <libapd.h>
#include "../libsiemens/strings.h"
#include "../libsiemens/lang.h"
#include "../libsiemens/graphics.h"
#include "conf_loader.h"
#include "config_data.h"

#define ITEMS 0x03

extern void kill_data(void *p, void (*func_p)(void *));

typedef struct
{
	CSM_RAM csm;
} MAIN_CSM;

CSM_RAM *under_idle;

const int minus11 =- 11;
int gui_id;
char **lgp;

enum
{
	lgpHdr,
	lgpShutdown,
	lgpReboot,
	lgpLock,
	lgpSelect,
	lgpBack
};

HEADER_DESC header = {0, 0, 0, 0, NULL, (int)"Options", LGP_NULL};

MENUITEM_DESC items[ITEMS] =
{
	{NULL, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{NULL, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{NULL, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
};

void Shutdown(GUI *data)
{
	SwitchPhoneOff();
	GeneralFuncF1(2);
}

void Reboot(GUI *data)
{
	RebootPhone();
	GeneralFuncF1(2);
}

void Lock(GUI *data)
{
	KbdLock();
	GeneralFuncF1(2);
}


const MENUPROCS_DESC procs[ITEMS] =
{
	Shutdown,
	Reboot,
	Lock,
};

void GHook(void *data, int cmd)
{
	if (cmd == TI_CMD_DESTROY)
	{
		gui_id = 0;
	}
}

int softkeys[] = {0, 1, 2};

SOFTKEY_DESC sk[] =
{
	{0x0018, 0x0000, (int)"Select"},
	{0x0001, 0x0000, (int)"Back"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

SOFTKEYSTAB skt =
{
	sk, 0
};

static MENU_DESC desc=
{
	8, NULL, GHook, NULL,
	softkeys,
	&skt,
	0x11,
	NULL,
	//Handler,
	items,   //Items
	procs,   //Procs
	ITEMS
};

void CreateTurnOffMenu(void)
{
	header.lgp_id = (int)lgp[lgpHdr];
	for (int i = 0; i < ITEMS; i++)
	{
		items[i].lgp_id_small = (int)lgp[lgpShutdown + i];
	}
	sk[0].lgp_id = (int)lgp[lgpSelect];
	sk[1].lgp_id = (int)lgp[lgpBack];
	patch_header_small(&header);
	if (cfg_enable)
	{
		static int icon_shutdown, icon_reboot, icon_lock;
		icon_shutdown = (int)cfg_icon_shutdown_path;
		icon_reboot   = (int)cfg_icon_reboot_path;
		icon_lock     = (int)cfg_icon_lock_path;
		
		items[0].icon = &icon_shutdown;
		items[1].icon = &icon_reboot;
		items[2].icon = &icon_lock;
	}
	else
	{
		static int icon_empty = ICON_EMPTY;
		for (int i = 0; i < ITEMS ; items[i++].icon = &icon_empty);
	}
	
	gui_id = CreateMenu(1, 0, &desc, &header, 0, ITEMS, NULL, NULL);
}

int KeyHook(int key, int type)
{
	if (type == KEY_DOWN && key == (int)cfg_key)
	{
		#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])
		void *icsm = FindCSMbyID(CSM_root()->idle_id);
		if (icsm)
		{
			if (IsGuiOnTop(idlegui_id(icsm)) && IsUnlocked()) //Если IdleGui на самом верху
			{
				CreateTurnOffMenu();
				return KEYHOOK_BREAK;
			}
		}
	}
	return KEYHOOK_NEXT;
}

int maincsm_onmessage(CSM_RAM* data, GBS_MSG* msg)
{	
	if (msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp_nocase(successed_config_filename, (char*)msg->data0) == 0)
			InitConfig();
	}
	return 1;
}

static void maincsm_oncreate(CSM_RAM *data){}

static void maincsm_onclose(CSM_RAM *csm)
{
	if (gui_id)
		GeneralFunc_flag1(gui_id, 0);
	FreeLang(&lgp);
	RemoveKeybMsgHook(KeyHook);
	kill_elf();
}

static unsigned short maincsm_name_body[140];

static struct
{
	CSM_DESC maincsm;
	WSHDR maincsm_name;
} MAINCSM =
{
	{
		maincsm_onmessage,
		maincsm_oncreate,
		#ifdef NEWSGOLD
			0,
			0,
			0,
			0,
		#endif
		maincsm_onclose,
		sizeof(MAIN_CSM),
		1,
		&minus11
	},
	{
		maincsm_name_body,
		NAMECSM_MAGIC1,
		NAMECSM_MAGIC2,
		0x0,
		139,
		0x0
	}
};

static void UpdateCSMname(void)
{
	wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "TurnOffD");
}

int main(const char *exe_path, const char *file_path)
{
	if (InitLang(cfg_lang_path, &lgp) == -1)
	{
		MsgBoxError(1, (int)"lang.txt not found!");
		kill_elf();
	}
	else
	{
		CSM_RAM *save_cmpc;
		char dummy[sizeof(MAIN_CSM)];
		LockSched();
		UpdateCSMname();
		InitConfig();
		save_cmpc = CSM_root()->csm_q->current_msg_processing_csm;
		CSM_root()->csm_q->current_msg_processing_csm = CSM_root()->csm_q->csm.first;
		CreateCSM(&MAINCSM.maincsm,dummy,0);
		CSM_root()->csm_q->current_msg_processing_csm = save_cmpc;
		UnlockSched();
		AddKeybMsgHook((void*)KeyHook);
	}
	return 0;
}
