#ifndef _MAIN_DATA_H_
	#define _MAIN_H_

#define MAX_ICONS 0x03

enum
{
	lgpMainSoft,
	lgpHeaderReboot,
	lgpHeaderShutdown,
	lgpHeaderExit,
	lgpSoftLeft,
	lgpSoftRight1
};

extern WSHDR *ws;
extern char **lgp;

extern IMGHDR *icons_sel[MAX_ICONS];
extern IMGHDR *icons_unsel[MAX_ICONS];

#endif
