#include <swilib.h>
#include "main.h"

#define ABOUT "APC - client for APD\n\xA9 FIL, 2013"

extern int softkeys[];

static SOFTKEY_DESC sk[]=
{
	{0x0018, 0x0000, (int)"OK"},
	{0x0001, 0x0000, (int)"Back"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB skt=
{
	sk, 0
};

static int OnKey(void *data, GUI_MSG *msg)
{
	if(msg->keys == 0x1 ||msg->keys == 0x18 || msg->keys == 0x3D)
		return 1;
	return 0;
}

static POPUP_DESC desc =
{
	8, OnKey, NULL, NULL,
	softkeys,
	&skt,
	0x1,
	LGP_NULL,
	NULL,
	0,
	FONT_MEDIUM,
	100,
	101,
	0
};

void CreateAboutGUI(void)
{
	sk[0].lgp_id = (int)lgp[lgpInfoOK];
	sk[1].lgp_id = (int)lgp[lgpInfoBack];
	CreatePopupGUI(0, NULL, &desc, (int)ABOUT);
}
