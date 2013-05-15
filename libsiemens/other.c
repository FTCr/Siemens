#include <swilib.h>

void IPC_SendMessage(IPC_REQ *ipc, const char *name_to, const char *name_from, const int mess, void *data)
{
	ipc->name_to   = name_to;
	ipc->name_from = name_from;
	ipc->data      = data;
	GBS_SendMessage(MMI_CEPID, MSG_IPC, mess, ipc);
}

void ExecEntrypoint(const char *name)
{
	typedef void (*voidfunc)(); 
	unsigned int addr = strtoul(name, 0, 16);
	voidfunc pp = (voidfunc)addr;
	SUBPROC((void*)pp);
}

void ExecShortcut(const char *name)
{
	unsigned int* addr = (unsigned int*)GetFunctionPointer((char*)name);
	if (addr)
	{
		typedef void (*voidfunc)(); 
		#ifdef NEWSGOLD          
			voidfunc pp = (voidfunc)*(addr + 4);
		#else
			voidfunc pp = (voidfunc)addr; 
		#endif 
		SUBPROC((void*)pp);
	}
}

void ExecFile(const char *path)
{
	unsigned int len = strlen(path);
	WSHDR *ws = AllocWS(len + 1);
	str_2ws(ws, path, len);
	ExecuteFile(ws, NULL, NULL);
	FreeWS(ws);
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

int *SetSWIHook(int swi_num, void *proc) 
{ 
	LockSched(); 
	unsigned int *addr = NULL;
	unsigned int *lib = (int *)(*((int *)(int*)LIB_TOP())); 
	if (lib == NULL) return NULL;
	addr = (unsigned int*)lib[swi_num];
	lib[swi_num] = (unsigned int)proc; 
	UnlockSched(); 
	return addr;
}

void DestroySWIHook(int swi_num, unsigned int *addr)
{
	LockSched();
	unsigned int *lib = (int *)(*((int *)(int*)LIB_TOP())); 
	if (lib == NULL) return;
	lib[swi_num] = (unsigned int)addr;
	UnlockSched();
}