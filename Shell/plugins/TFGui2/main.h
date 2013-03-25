#ifndef _MAIN_H_
	#define _MAIN_H_
	
#define MENU_ITEMS 0x05

enum
{
	lgpMainSoft,
	
	lgpHeader,
	lgpSoftLeft,
	lgpSoftRight,
	
	lgpMenuShutdown,
	lgpMenuShutdown_add,
	lgpMenuReboot,
	lgpMenuReboot_add,
	lgpMenuExit,
	lgpMenuExit_add,
	lgpMenuRestart,
	lgpMenuRestart_add,
	lgpMenuLock,
	lgpMenuLock_add
};

extern char **lgp;

extern IMGHDR *icons[MENU_ITEMS];
extern char *strings1[MENU_ITEMS], *strings2[MENU_ITEMS];

extern WSHDR *ws;

#endif
