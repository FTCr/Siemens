#include <swilib.h>
#include "../../../libshell/plugins.h"
#include "../../../libshell/other.h"

CSM_RAM *under_idle;

const int minus11 = -11;

unsigned int killed;

typedef struct
{
	CSM_RAM csm;
} MAIN_CSM;

int maincsm_onmessage(CSM_RAM* data, GBS_MSG* msg)
{
	if (msg->msg == MSG_IPC)
	{
		IPC_REQ *ipc = (IPC_REQ*)msg->data0;
		if (ipc)
		{
			if (strcmp(ipc->name_to, "Shell") == 0)
			{
				switch (msg->submess)
				{
					case 0x01:
						killed = 1;
						CloseCSM(data->id);
					break;
				}
			}
		}
	}
	return 1;
}

static void maincsm_oncreate(CSM_RAM *data){}

static void maincsm_onclose(CSM_RAM *csm)
{
	if (!killed)
		Close();
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
		0,
		139,
		0
	}
};

static void UpdateCSMname(void){wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Shell");}

int main(PLUGIN_S4T *plg)
{
	CSM_RAM *save_cmpc;
	char dummy[sizeof(MAIN_CSM)];
	UpdateCSMname();  
	LockSched();
	save_cmpc = CSM_root()->csm_q->current_msg_processing_csm;
	CSM_root()->csm_q->current_msg_processing_csm = CSM_root()->csm_q->csm.first;
	CreateCSM(&MAINCSM.maincsm,dummy,0);
	CSM_root()->csm_q->current_msg_processing_csm = save_cmpc;
	UnlockSched();
	return 0;
}
