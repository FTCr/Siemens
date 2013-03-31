#include <swilib.h>
#include "../libsiemens/other.h"
#include "conf_loader.h"

extern void kill_data(void *p, void (*func_p)(void *));

CSM_RAM *under_idle;

const int minus11=-11;
int is_cpu_hi;

typedef struct
{
	CSM_RAM csm;
}MAIN_CSM;

GBSTMR tmr;

void SetCPUClockLow(void)
{
	if (IsTimerProc(&tmr))
		GBS_DelTimer(&tmr);
	SetCpuClockLow(2);
	is_cpu_hi = 0;
}

int KeyHook(int key, int type)
{
	switch (type)
	{
		case KEY_DOWN:
			goto LP;
		break;
		case LONG_PRESS:
			LP:
				if (!is_cpu_hi)
				{
					SetCpuClockTempHi(2);
					is_cpu_hi = 1;
				}
		break;
		case KEY_UP:
			if (is_cpu_hi)
			{
				GBS_StartTimerProc(&tmr, TMR_6_SEC, (void*)SetCPUClockLow);
			}
		break;
	}
	return KEYHOOK_NEXT;
}

int maincsm_onmessage(CSM_RAM* data, GBS_MSG* msg){return 1;}

static void maincsm_oncreate(CSM_RAM *data)
{
	if (GetCPUClock() >= 104)
		is_cpu_hi = 1;
}

static void maincsm_onclose(CSM_RAM *csm)
{
	SetCPUClockLow();
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
	wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "CPUControl");
}

int main(void)
{
	CSM_RAM *save_cmpc;
	char dummy[sizeof(MAIN_CSM)];
	LockSched();
	UpdateCSMname();
	save_cmpc = CSM_root()->csm_q->current_msg_processing_csm;
	CSM_root()->csm_q->current_msg_processing_csm = CSM_root()->csm_q->csm.first;
	CreateCSM(&MAINCSM.maincsm,dummy,0);
	CSM_root()->csm_q->current_msg_processing_csm = save_cmpc;
	UnlockSched();
	AddKeybMsgHook((void*)KeyHook);
	return 0;
}
