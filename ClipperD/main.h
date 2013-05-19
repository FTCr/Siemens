#ifndef _MAIN_H_
	#define _MAIN_H_

#define IPC_MY_NAME "Clipper"
	
typedef struct
{
	void *next;
	void *prev;
	WSHDR *ws;
}CLIPBOARD_LIST;

extern const int minus11;

extern CLIPBOARD_LIST *top;
extern CLIPBOARD_LIST *last;
extern unsigned int total;

extern char **lgp;

enum
{
	lgpHeader,
	lgpClear,
	lgpExit
};

extern unsigned int not_reading;


void Clear(void);

#endif
