#include <swilib.h>
#include <libapd.h>
#include "../libsiemens/strings.h"
#include "../libsiemens/obs.h"
#include "conf_loader.h"
#include "config_data.h"

extern void kill_data(void *p, void (*func_p)(void *));

CSM_RAM *under_idle;

const int minus11=-11;

typedef struct
{
	CSM_RAM csm;
}MAIN_CSM;

int KeyHook(int key, int type)
{
	if (type == KEY_DOWN || type == LONG_PRESS)
	{
		switch (key)
		{
			case VOL_UP_BUTTON:
				if (APlayer_GetPlayStatus() == APLAYER_PLAY)
				{
					APlayer_SetVolume(APlayer_GetVolume() + 1);
					return KEYHOOK_BREAK;
				}
			break;
			case VOL_DOWN_BUTTON:
				if (APlayer_GetPlayStatus() == APLAYER_PLAY)
				{
					APlayer_SetVolume(APlayer_GetVolume() - 1);
					return KEYHOOK_BREAK;
				}
			break;
		}
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
	return 1;
}

static void maincsm_oncreate(CSM_RAM *data){}

static void maincsm_onclose(CSM_RAM *csm)
{
	extern void APlayer_Destroy();
	APlayer_Destroy();
	RemoveKeybMsgHook(KeyHook);
	kill_elf();
}

static unsigned short maincsm_name_body[140];

static const struct
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
	wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "APD");
}

int main(const char *exe_path, const char *file_path)
{
	if (!APlayer_IsLaunch())
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
		extern unsigned int APlayer_Init(const char *mus_dir, const char *pls_dir);
		APlayer_Init(cfg_mus_dir, cfg_pls_dir);
		AddKeybMsgHook((void*)KeyHook);
	}
	else
	{
		unsigned int uid = GetExtUidByFileName(file_path);
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
		kill_elf();
	}
	return 0;
}
