#include <swilib.h>
#include <xtask_ipc.h>
#include "../libsiemens/graphics.h"
#include "../libsiemens/other.h"
#include "main.h"
#include "config_data.h"

typedef struct
{
	CSM_RAM csm;
	unsigned int gui_id;
} MAIN_CSM;

unsigned int gui_csm_id;

HEADER_DESC header = {0, 0, 0, 0, NULL, 0, LGP_NULL};

int softkeys[] = {0, 1, 2};

SOFTKEY_DESC sk[] =
{
	{0x0018, 0x0000, (int)"Clear"},
	{0x0001, 0x0000, (int)"Cancel"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

SOFTKEYSTAB skt =
{
	sk, 0
};

int OnKey(void *data, GUI_MSG *msg)
{
	const int Key   = msg->gbsmsg->submess;
	const int Style = msg->gbsmsg->msg;
	
	if (msg->keys == 0x0018)
	{
		Clear();
		ClearClipboard();
		total = 0;
		Menu_SetItemCountDyn(data, total);
		RefreshGUI();
	}
	
	unsigned int Get(CLIPBOARD_LIST **ptr)
	{
		unsigned int i = GetCurMenuItem(data);
		
		CLIPBOARD_LIST *p = top;
		unsigned int j = 0;
		while (j < total - 1 - i)
		{
			p = p->next;
			j++;
		}
		*ptr = p;
		return j;
	}
	
	void Cut(CLIPBOARD_LIST *ptr, unsigned int j)
	{
		CLIPBOARD_LIST *p1, *p2;
		if (top->next)
		{
			if (!j)
			{
				top = top->next;
				top->prev = NULL;
			}
			else
			{
				p1 = ptr->prev;
				if (ptr->next)
				{
					p2 = ptr->next;
					p2->prev = p1;
					p1->next = ptr->next;
				}
				else
				{
					p1->next = NULL;
				}
				if (j == total - 1)
				{
					last = p1;
				}
			}
			FreeWS(ptr->ws);
			mfree(ptr);
		}
		else
		{
			ClearClipboard();
			FreeWS(top->ws);
			mfree(top);
			top = NULL;
		}
	}
	
	if (Style == KEY_DOWN || Style == LONG_PRESS)
	{
		if (Key == '#')
		{
			if (total)
			{
				CLIPBOARD_LIST *ptr;
				unsigned int j = Get(&ptr);
				Cut(ptr, j);
				total--;
				Menu_SetItemCountDyn(data, total);
				RefreshGUI();
			}
		}
		else if (Key == ENTER_BUTTON)
		{
			if (total > 1)
			{
				CLIPBOARD_LIST *ptr;
				
				unsigned int j = Get(&ptr);
				WSHDR *ws = AllocWS(ptr->ws->wsbody[0] + 1);
				wstrcpy(ws, ptr->ws);
				Cut(ptr, j);
				
				ptr = top;
				while (1)
				{
					if (ptr->next) ptr = ptr->next;
					else break;
				}
				
				CLIPBOARD_LIST *do_top = malloc(sizeof(CLIPBOARD_LIST));
				ptr->next = do_top;
				do_top->prev = ptr;
				do_top->next = NULL;
				do_top->ws = ws;
				last = do_top;
				
				//иначе пикает :-(
				WSHDR *src = AllocWS(ws->wsbody[0] + 1);
				wsprintf(src, "%w", ws);
				CopyWsToClipboard(src);
				FreeWS(src);
				
				not_reading = 1;
				
				SetCursorToMenuItem(data, 0);
				RefreshGUI();
			}
		}
	}
	return 0;
}

void GHook(void *data, int cmd)
{
	if (cmd == TI_CMD_FOCUS)
	{
		DisableIDLETMR();
	}
	if (cmd == TI_CMD_REDRAW || cmd == TI_CMD_CREATE)
	{
		if (total)
		{
			sk[0].lgp_id = (int)lgp[lgpClear];
			sk[2].lgp_id = (int)LGP_DOIT_PIC;
		}
		else
		{
			sk[0].lgp_id = (int)"";
			sk[2].lgp_id = (int)" ";
		}
	}
}

void Handler(void *data, int cur_item, void *user_pointer)
{
	CLIPBOARD_LIST *p = top;
	
	while(1) //goto last element
	{
		if (p->next) p = p->next;
		else break;
	}
	
	for (int i = 0; i < cur_item; i++)
		p = p->prev;
	
	
	void *item = AllocMenuItem(data);
	WSHDR *ws  = AllocMenuWS(data, p->ws->wsbody[0]);
	wstrcpy(ws, p->ws);
	SetMenuItemText(data, item, ws, cur_item);
}

MENU_DESC desc =
{
	8, OnKey, GHook, NULL,
	softkeys,
	&skt,
	0x10,
	Handler,
	NULL,   //Items
	NULL,   //Procs
	0   //n
};

void maincsm_oncreate(CSM_RAM *data)
{
	MAIN_CSM *csm = (MAIN_CSM*)data;
	patch_header(&header);
	static int icon = (int)cfg_icon_path;
	header.icon = &icon;
	header.lgp_id = (int)lgp[lgpHeader];
	sk[1].lgp_id  = (int)lgp[lgpExit];
	csm->gui_id = CreateMenu(0, 0, &desc, &header, 0, total, 0, 0);
}

void maincsm_onclose(CSM_RAM *data)
{
	gui_csm_id = 0;
}


static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
	MAIN_CSM *csm = (MAIN_CSM*)data;
	if (msg->msg == MSG_GUI_DESTROYED)
	{
		if ((unsigned int)msg->data0 == csm->gui_id)
		{
			csm->csm.state =- 3;
		}
	}
	return 1;
}

unsigned short maincsm_name_body[140];

const struct
{
	CSM_DESC maincsm;
	WSHDR maincsm_name;
}MAINCSM =
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


void UpdateCSMname(void)
{
	wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "%t", lgp[lgpHeader]);
}

void CreateClipperGUI(void)
{
	if (gui_csm_id)
	{
		static IPC_REQ ipc;
		IPC_SendMessage(&ipc, IPC_XTASK_NAME, IPC_MY_NAME, IPC_XTASK_SHOW_CSM, (void*)gui_csm_id);
	}
	else
	{
		char dummy[sizeof(MAIN_CSM)];
		UpdateCSMname();
		gui_csm_id = CreateCSM(&MAINCSM.maincsm,dummy,0);
	}
}