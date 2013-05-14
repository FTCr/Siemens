#include <swilib.h>
#include <libapd.h>
#include <libid3.h>
#include "../libsiemens/graphics.h"
#include "../libsiemens/lang.h"
#include "../libsiemens/strings.h"
#include "main.h"
#include "conf_loader.h"
#include "config_data.h"
#include "tab_tracks.h"
#include "tab_playlists.h"

typedef struct
{
	CSM_RAM csm;
} MAIN_CSM;

const int minus11 =- 11;

static void *tab_gui;
static unsigned int gui_id;

char **lgp;

char str_hdr[256];
char str_hdr2[32];

static const int tab_icons[2][4]=
{
	0x487, 0x487, 0x487, 0x487,
	0x489, 0x489, 0x489, 0x489,
};

static HEADER_DESC header = {0, 0, 0, 0, NULL, (int)str_hdr, (int)str_hdr2};

int softkeys[] = {0, 1, 2};

void SetHeader(void)
{
	DIR_ENTRY_LIST *ptr = APlayer_GetCurPtr();
	if (ptr)
	{
		ID3 *id3 = ptr->data;
		if (id3)
		{
			if (id3->artist && id3->title)
			{
				WSHDR *ws = AllocWS(id3->artist->wsbody[0] + id3->title->wsbody[0] + 5);
				wsprintf(ws, "%w - %w", id3->artist, id3->title);
				ws2ascii(str_hdr, ws);
				FreeWS(ws);
			}
			else goto NAME;
		}
		else
		{
			NAME:
				if (GetFileNameWithoutExtByPath(str_hdr, ptr->path) == -1)
					strcpy(str_hdr, lgp[lgpHeader]);
		}
	}
	else
	{
		strcpy(str_hdr, lgp[lgpHeader]);
	}
	
	if (APlayer_GetTotalTracks())
		sprintf(str_hdr2, "%d", APlayer_GetTrack());
	else
		strcpy(str_hdr2, "");
}

static void GHook_TabGUI(void *data, int cmd)
{
	if (cmd == TI_CMD_FOCUS)
	{
		SetHeader();
		DisableIDLETMR();
	}
	if (cmd == TI_CMD_REDRAW)
	{
		int n = GetCursorTab(data);
		if (n == 0)
			SetMidIcon();
	}
}

static int OnKey_TabGUI(void *data, GUI_MSG *msg)
{
	const int Key   = msg->gbsmsg->submess;
	const int Style = msg->gbsmsg->msg;
	
	if (Style == KEY_DOWN || Style == LONG_PRESS)
	{
		switch (Key)
		{
			case LEFT_BUTTON: case RIGHT_BUTTON:
				RefreshGUI();
			break;
		}
	}
	return 0;
}

static const TABGUI_DESC TabGUI_desc =
{
	8, OnKey_TabGUI, GHook_TabGUI, NULL,
	NULL,
	NULL
};

static void Refresh(GUI *data)
{
	RefreshGUI();
}

void RefreshMainGUI(void)
{
	Refresh(tab_gui);
}

void maincsm_oncreate(CSM_RAM *data)
{
	MAIN_CSM *csm = (MAIN_CSM*)data;
	
	void *m_gui;
	void *ma = malloc_adr();
	void *mf = mfree_adr();
	
	/*void *tab_gui*/tab_gui = GetTabGUI(ma, mf);
	SetGUIDefinition(tab_gui, &TabGUI_desc);
	
	patch_header(&header);
	header.lgp_id   = (int)str_hdr;
	header.lgp_null = (int)str_hdr2;
	
	tab_tracks_gui = m_gui = GetMultiLinesMenuGUI(ma, mf);
	SetMenuToGUI(m_gui, &Desc_TabTracks);
	SetMenuItemCount(m_gui, APlayer_GetTotalTracks());
	SetCursorToMenuItem(m_gui, 0);
	SetHeaderToMenu(m_gui, &header, ma);
	SetGUIToTabGUI(tab_gui, 0, tab_icons[0], m_gui);
	
	DIR_ENTRY_LIST *top = NULL;
	FIND_UIDS fu;
	fu.uid1 = UID_M3U;
	int total = FindFilesRec(&top, APlayer_GetPlsDir(), &fu, NULL);
	
	m_gui = GetMultiLinesMenuGUI(ma, mf);
	SetMenuToGUI(m_gui, &Desc_TabPlaylists);
	MenuSetUserPointer(m_gui, top);
	SetMenuItemCount(m_gui, total);
	SetCursorToMenuItem(m_gui, 0);
	SetHeaderToMenu(m_gui, &header, ma);
	SetGUIToTabGUI(tab_gui, 1, tab_icons[1], m_gui);
	
	
	SetCursorTab(tab_gui, 0);
	UpdateTab1stItem(tab_gui, 0);
	m_gui = tab_gui;
	gui_id = CreateGUI(tab_gui);
}

void maincsm_onclose(CSM_RAM *data)
{
	if (cfg_stop_playing)
		APlayer_Stop();
	FreeLang(&lgp);
	kill_elf();
}


int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
	MAIN_CSM *csm = (MAIN_CSM*)data;
	if (msg->msg == MSG_GUI_DESTROYED)
	{
		if ((unsigned int)msg->data0 == gui_id)
		{
			csm->csm.state =- 3;
			CloseCSM(csm->csm.id); //думаю это здесь и не нужно
		}
	}
	if (msg->msg == MSG_IPC)
	{
		IPC_REQ *ipc = (IPC_REQ*)msg->data0;
		if (ipc)
		{
			if (strcmp(ipc->name_to, APD_IPC_NAME) == 0)
			{
				switch (msg->submess)
				{
					//смена трэка
					case APD_IPC_UPDATE_TRACK:
						if (IsGuiOnTop(gui_id))
						{
							RefreshGUI();
						}
					break;
					case APD_IPC_DESTROY:
						CloseCSM(data->id);
					break;
				}
			}
		}
	}
	if (msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp_nocase(successed_config_filename, (char *)msg->data0) == 0)
			InitConfig();
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

int main(const char *path)
{
	char dir[128], l_path[256];
	unsigned int err;
	
	GetDirByPath(dir, path);
	strcat(dir, "\\");
	
	strcpy(l_path, dir);
	strcat(l_path, "lang.txt");
	if (InitLang(l_path, &lgp) == -1)
	{
		MsgBoxError(1, (int)"Not found lang.txt!");
		kill_elf();
		return 0;
	}
	if (!APlayer_IsLaunch())
	{
		MsgBoxError(1, (int)"APD not started!");
		kill_elf();
		return 0;
	}
	InitConfig();
	char dummy[sizeof(MAIN_CSM)];
	UpdateCSMname();
	LockSched();
	CreateCSM(&MAINCSM.maincsm,dummy,0);
	UnlockSched();
	return 0;
}

