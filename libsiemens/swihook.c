#include <swilib.h>

int *SetSWIHook(int swi_num, void *proc) 
{ 
	LockSched(); 
	unsigned int *addr = NULL;
	unsigned int *lib = (int *)(*((int *)(int*)LIB_TOP())); 
	if (lib == NULL) return NULL;
	addr = (unsigned int*)lib[swi_num];
	lib[swi_num] = (unsigned int)proc; 
	UnlockSched(); 
	
}

void DestroySWIHook(int swi_num, unsigned int *addr)
{
	LockSched();
	unsigned int *lib = (int *)(*((int *)(int*)LIB_TOP())); 
	if (lib == NULL) return;
	lib[swi_num] = (unsigned int)addr;
	UnlockSched();
}
