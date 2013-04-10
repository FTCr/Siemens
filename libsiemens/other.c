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

void patch_rect(RECT*rc, int x, int y, int x2, int y2)
{
	rc->x  = x;
	rc->y  = y;
	rc->x2 = x2;
	rc->y2 = y2;
}

void patch_header(HEADER_DESC* head)
{
	((HEADER_DESC*)head)->rc.x = 0;
	((HEADER_DESC*)head)->rc.y = YDISP;
	((HEADER_DESC*)head)->rc.x2 = ScreenW()-1;
	((HEADER_DESC*)head)->rc.y2 = HeaderH() + YDISP-1;
}

void patch_header_small(HEADER_DESC* head)
{
	head->rc.x  = 3;
	head->rc.x2 = ScreenW()-6;
	#ifndef ELKA
		head->rc.y  = YDISP + 0x18;
		head->rc.y2 = YDISP + 0x18 + 0x13;
	#else
		head->rc.y  = YDISP + 0x23;
		head->rc.y2 = YDISP + 0x23+0x22;
	#endif
}

void patch_input(INPUTDIA_DESC* inp)
{
	((INPUTDIA_DESC*)inp)->rc.x  = 0;
	((INPUTDIA_DESC*)inp)->rc.y  = HeaderH()+1+YDISP;
	((INPUTDIA_DESC*)inp)->rc.x2 = ScreenW()-1;
	((INPUTDIA_DESC*)inp)->rc.y2 = ScreenH()-SoftkeyH()-1;
}
