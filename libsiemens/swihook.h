#ifndef _LIBS_SWIHOOK_H_
	#define _LIBS_SWIHOOK_H_

#define SWI_KBDLOCK   0xA9
#define SWI_KBDUNLOCK 0xB9

int *SetSWIHook(int swi_num, void *proc);
void DestroySWIHook(int swi_num, unsigned int *addr);
	
#endif
