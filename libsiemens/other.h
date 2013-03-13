#ifndef _LIBS_OTHER_H_
	#define _LIBS_OTHER_H_

#define TMR_6_SEC 1300

void ExecFunc(const char *name);
void ExecFile(const char *path);

void DelTimer(GBSTMR *tmr);
	
unsigned int GetCSMIDFromAddr(const char *addr);
#endif
