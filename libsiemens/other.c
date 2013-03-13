#include <swilib.h>
#include "string_utils.h"

void ExecFunc(const char *name)
{
	typedef void (*voidfunc)();
	voidfunc pp;
	if (name[0] == '0' && name[1] == 'x') //entrypoint
	{
		unsigned int addr = strtoul(name, 0, 16);
		pp = (voidfunc)addr;
	}
	else //shortcut
	{
		unsigned int *addr = (unsigned int*)GetFunctionPointer((char*)name);
		if (addr)
		{
			typedef void (*voidfunc)();
		#ifdef NEWSGOLD
			pp = (voidfunc)*(addr + 4);
		#else
			pp = (voidfunc)addr;
		#endif
		}
	}
	SUBPROC((void*)pp);
}

void ExecFile(const char *path)
{
	unsigned int len = strlen(path);
	WSHDR *ws = AllocWS(len + 1);
	str_2ws(ws, path, len);
	ExecuteFile(ws, NULL, NULL);
	FreeWS(ws);
}


void DelTimer(GBSTMR *tmr)
{
	if (IsTimerProc(tmr))
	{
		GBS_StopTimer(tmr);
		GBS_DelTimer(tmr);
	}
}


unsigned int GetCSMIDFromAddr(const char *addr)
{
	CSM_RAM *p;
	p = CSM_root()->csm_q->csm.first;
	while (p)
	{
		void *player_csm_addr = NULL;
		player_csm_addr = (void*)strtoul(addr, NULL, 16);
		if (p->constr == player_csm_addr) break;
		p = p->next;
	}
	if (p) return p->id;
	return 0;
}
