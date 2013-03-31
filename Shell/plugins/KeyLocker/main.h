#ifndef _MAIN_H_
	#define _MAIN_H_

#define DLOCK tmr.param5

extern GBSTMR tmr;
extern unsigned int is_killed;

void DLock(void);
	
#endif
