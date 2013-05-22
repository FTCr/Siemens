#include <swilib.h>
#include <libapd.h>
#include "../libsiemens/strings.h"
#include "../libsiemens/obs.h"
#include "conf_loader.h"
#include "config_data.h"

CSM_RAM *under_idle;
int (*old_icsm_onMessage)(CSM_RAM*, GBS_MSG *msg);

const int minus11=-11;

typedef struct
{
	CSM_RAM csm;
}MAIN_CSM;

int KeyHook(int key, int type)
{
	static int ptt_flag;
	switch (type)
	{
		case KEY_DOWN:
			switch (key)
			{
				case PTT_BUTTON: APlayer_Toggle(); return KEYHOOK_BREAK;
				case VOL_UP_BUTTON: goto UP;
				case VOL_DOWN_BUTTON: goto DOWN;
			}
		break;
		case LONG_PRESS:
			switch (key)
			{
				case PTT_BUTTON: ptt_flag = 1; return KEYHOOK_BREAK;
				case VOL_UP_BUTTON:
					UP:
						if (APlayer_GetPlayStatus() == APLAYER_PLAY)
						{
							APlayer_SetVolume(APlayer_GetVolume() + 1);
							return KEYHOOK_BREAK;
						}
				break;
				case VOL_DOWN_BUTTON:
					DOWN:
						if (APlayer_GetPlayStatus() == APLAYER_PLAY)
						{
							APlayer_SetVolume(APlayer_GetVolume() - 1);
							return KEYHOOK_BREAK;
						}
				break;
			}
		break;
		case KEY_UP:
			if (key == PTT_BUTTON && ptt_flag == 1)
			{
				ptt_flag = 0;
				APlayer_NextTrack();
				return KEYHOOK_BREAK;
			}
		break;
	}
	return KEYHOOK_NEXT;
}

int maincsm_onmessage(CSM_RAM* data, GBS_MSG* msg)
{
	if (msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp_nocase(successed_config_filename, (char *)msg->data0) == 0)
			InitConfig();
	}
	if (msg->msg == MSG_IPC)
	{
		IPC_REQ *ipc = (IPC_REQ*)msg->data0;
		if (ipc)
		{
			if (strcmp_nocase(ipc->name_to, APD_IPC_NAME) == 0)
			{
				switch (msg->submess)
				{
					case APD_IPC_UPDATE_PLAYSTATUS:
						GBS_SendMessage(MMI_CEPID, MSG_REFRESH_ICONBAR);
					break;
				}
			}
		}
	}
	return 1;
}

int IDLEcsm_onmessage(CSM_RAM *icsm, GBS_MSG *msg)
{
	static unsigned int flag;
	if ((msg->msg == MSG_STATE_OF_CALL || msg->msg == MSG_INCOMMING_CALL) && msg->submess)
	{
		if (APlayer_GetPlayStatus() == APLAYER_PLAY)
		{
			flag = 1;
			APlayer_FakePause();
		}
	}
	if (msg->msg == MSG_END_CALL)
	{
		if (flag == 1)
		{
			flag = 0;
			APlayer_FakePlay();
		}
	}
	
	return old_icsm_onMessage(icsm, msg);
}

static void maincsm_oncreate(CSM_RAM *data){}

static void maincsm_onclose(CSM_RAM *csm)
{
	LockSched();
	CSM_RAM *csm_ram = FindCSMbyID(CSM_root()->idle_id);
	((CSM_DESC*)csm_ram->constr)->onMessage = old_icsm_onMessage;
	UnlockSched();
	extern void APlayer_Destroy();
	APlayer_Destroy();
	RemoveKeybMsgHook(KeyHook);
	kill_elf();
}

void IconBar(short* num)
{
	if (cfg_enable_iconbar)
	{
		int icon = 0;
		switch (APlayer_GetPlayStatus())
		{
			case APLAYER_PLAY:
				icon = cfg_play_icon;
			break;
			case APLAYER_STOP:
				icon = cfg_stop_icon;
			break;
			case APLAYER_PAUSE:
				icon = cfg_pause_icon;
			break;
			case APLAYER_FORWARD:
				icon = cfg_forward_icon;
			break;
			case APLAYER_REWIND:
				icon = cfg_rewind_icon;
			break;
		}
		AddIconToIconBar(icon, num);
	}
}

static unsigned short maincsm_name_body[140];

static struct
{
	CSM_DESC maincsm;
	WSHDR maincsm_name;
	ICONBAR_H iconbar_handler;
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
	},
	{
		"IconBar",
		(int)IconBar
	}
};

static void UpdateCSMname(void)
{
	wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "APD");
}

int main(const char *exe_path, const char *file_path)
{
	if (!APlayer_IsLaunch())
	{
		CSM_RAM *csm_ram;
		char dummy[sizeof(MAIN_CSM)];
		LockSched();
		UpdateCSMname();
		InitConfig();
		csm_ram = CSM_root()->csm_q->current_msg_processing_csm;
		CSM_root()->csm_q->current_msg_processing_csm = CSM_root()->csm_q->csm.first;
		CreateCSM(&MAINCSM.maincsm, dummy, 0);
		CSM_root()->csm_q->current_msg_processing_csm = csm_ram;
		
		csm_ram = FindCSMbyID(CSM_root()->idle_id);
		old_icsm_onMessage = ((CSM_DESC*)csm_ram->constr)->onMessage;
		((CSM_DESC*)csm_ram->constr)->onMessage = IDLEcsm_onmessage;
		
		UnlockSched();
		extern unsigned int APlayer_Init(const char *mus_dir, const char *pls_dir);
		APlayer_Init(cfg_mus_dir, cfg_pls_dir);
		AddKeybMsgHook((void*)KeyHook);
	}
	else
	{
		unsigned int uid = GetExtUidByFileName(file_path);
		if (uid)
		{
			if (uid == UID_M3U)
			{
				APlayer_ClearPlayList();
				APlayer_OpenPlayList(file_path);
			}
			else if (uid == UID_MP3 || uid == UID_WAV || uid == UID_AAC)
			{
				APlayer_OpenFile(file_path);
			}
			else
			{
				MsgBoxError(1, (int)"File is not supported!");
			}
		}
		else
		{
			MsgBoxError(1, (int)"APD already started!");
		}
		kill_elf();
	}
	return 0;
}
