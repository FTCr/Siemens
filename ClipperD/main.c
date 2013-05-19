#include <swilib.h>
#include "../libsiemens/other.h"
#include "../libsiemens/strings.h"
#include "../libsiemens/lang.h"
#include "main.h"
#include "gui.h"
#include "conf_loader.h"
#include "config_data.h"

extern void kill_data(void *p, void (*func_p)(void *));

CSM_RAM *under_idle;

const int minus11=-11;

typedef struct
{
	CSM_RAM csm;
}MAIN_CSM;

CLIPBOARD_LIST *top;
CLIPBOARD_LIST *last;
unsigned int total;

char **lgp;

unsigned int not_reading;

int KeyHook(int key, int style)
{
	#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])
	#define func(x) style == x && key == (int)cfg_key && !IsCalling() && IsUnlocked()
	
	if (cfg_enable_keyhook)
	{
		if (cfg_style == 0)
		{
			if (func(KEY_DOWN))
			{
				goto LAUNCH;
			}
		}
		else
		{
			static int flag;
			void *icsm = FindCSMbyID(CSM_root()->idle_id);
			if (func(KEY_DOWN))
			{
				if (flag == 1)
				{
					flag = 0;
				}
				else
				{
					flag = 1;
					return KEYHOOK_BREAK;
				}
			}
			if (func(LONG_PRESS))
			{
				LAUNCH:
					if (cfg_enable_of_idlescreen)
					{
						if (IsGuiOnTop(idlegui_id(icsm)))
						{
							CreateClipperGUI();
							return KEYHOOK_BREAK;
						}
					}
					else
					{
						if (!IsGuiOnTop(idlegui_id(icsm)))
						{
							CreateClipperGUI();
							return KEYHOOK_BREAK;
						}
					}
			}
			else if (func(KEY_UP) && flag == 1)
			{
				GBS_SendMessage(MMI_CEPID, KEY_DOWN, cfg_key);
			}
		}
	}
	return KEYHOOK_NEXT;
}

int maincsm_onmessage(CSM_RAM* data, GBS_MSG* msg)
{
	if (msg->msg == MSG_IPC)
	{
		IPC_REQ *ipc = (IPC_REQ*)msg->data0;
		if (ipc)
		{
			if (strcmp(ipc->name_to, IPC_MY_NAME) == 0)
			{
				if (msg->submess != data->id)
				{
					LockSched();
					CloseCSM(msg->submess);
					SUBPROC((void*)CreateClipperGUI);
					UnlockSched();
				}
			}
		}
	}
	if (msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp_nocase(successed_config_filename, (char *)msg->data0) == 0)
			InitConfig();
	}
	if (total < cfg_max_history)
	{
		static WSHDR *clip;
		if (not_reading)
		{
			clip = GetClipBoard();
			not_reading = 0;
		}
		else
		{
			if (clip != GetClipBoard())
			{
				clip = GetClipBoard();
				if (total)
				{
					CLIPBOARD_LIST *p = top;
					while (1)
					{
						if (p->next) p = p->next;
						else break;
					}
					p->next = malloc(sizeof(CLIPBOARD_LIST));
					p = p->next;
					last->next = p;
					
					p->prev = last;
					p->next = NULL;
					
					p->ws = AllocWS(clip->wsbody[0] + 1);
					wstrcpy(p->ws, clip);
					last = p;
					
				}
				else
				{
					top = malloc(sizeof(CLIPBOARD_LIST));
					top->prev = NULL;
					top->next = NULL;
					last = top;
					top->ws = AllocWS(clip->wsbody[0] + 1);
					wstrcpy(top->ws, clip);
				}
				total++;
			}
		}
	}
	return 1;
}

static void maincsm_oncreate(CSM_RAM *data)
{
	static IPC_REQ ipc;
	IPC_SendMessage(&ipc, IPC_MY_NAME, IPC_MY_NAME, data->id, NULL);
}

void Clear(void)
{
	CLIPBOARD_LIST *next;
	while (top)
	{
		next = top->next;
		FreeWS(top->ws);
		mfree(top);
		top = next;
	}
}

static void maincsm_onclose(CSM_RAM *csm)
{
	if (gui_csm_id)
		CloseCSM(gui_csm_id);
	FreeLang(&lgp);
	Clear();
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
	wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "ClipperD");
}

int main(const char *exe_path, const char *file_path)
{
	InitConfig();
	
	if (InitLang(cfg_lang_path, &lgp) == -1)
	{
		MsgBoxError(1, (int)"lang.txt not found!");
		kill_elf();
	}
	else
	{
		LockSched();
		UpdateCSMname();
		char dummy[sizeof(MAIN_CSM)];
		CSM_RAM *csm_ram = CSM_root()->csm_q->current_msg_processing_csm;
		CSM_root()->csm_q->current_msg_processing_csm = CSM_root()->csm_q->csm.first;
		CreateCSM(&MAINCSM.maincsm, dummy, 0);
		CSM_root()->csm_q->current_msg_processing_csm = csm_ram;
		UnlockSched();
		AddKeybMsgHook((void*)KeyHook);
	}
	return 0;
}
