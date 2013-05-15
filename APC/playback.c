#include <swilib.h>
#include "../libsiemens/graphics.h"
#include "../libapd/libapd.h"
#include "main.h"

#define PLAYBACK_ITEMS_N 0x02

const int icon_sel   = ICON_RB_SEL;
const int icon_unsel = ICON_RB_UNSEL;

static HEADER_DESC header = {0, 0, 0, 0, NULL, (int)"Playback", LGP_NULL};

static MENUITEM_DESC items[PLAYBACK_ITEMS_N] =
{
	{NULL, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{NULL, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
};

static void GHook(void *data, int cmd)
{
	if (cmd == TI_CMD_CREATE)
	{
		SetCursorToMenuItem(data, APlayer_GetPlayBack());
	}
	if (cmd == TI_CMD_FOCUS)
	{
		DisableIDLETMR();
	}
}

static void Repeat(GUI *data)
{
	APlayer_SetPlayBack(APLAYER_PLAYBACK_REPEAT);
	GeneralFuncF1(2);
}

static void Random(GUI *data)
{
	APlayer_SetPlayBack(APLAYER_PLAYBACK_RANDOM);
	GeneralFuncF1(2);
}

static const MENUPROCS_DESC procs[PLAYBACK_ITEMS_N]=
{
	Repeat,
	Random
};

extern int softkeys[];

static SOFTKEY_DESC sk[] =
{
	{0x0018, 0x0000, (int)"Select"},
	{0x0001, 0x0000, (int)"Back"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB skt =
{
	sk, 0
};

static MENU_DESC desc=
{
	8, NULL, GHook, NULL,
	softkeys,
	&skt,
	0x11,
	NULL,
	//Handler,
	items,   //Items
	procs,   //Procs
	PLAYBACK_ITEMS_N //n
};

void CreatePlaybackMenu(void)
{
	patch_header_small((HEADER_DESC*)(&header));
	
	int i = APlayer_GetPlayBack();
	
	items[i].icon = (int*)&icon_sel;
	for (int j = 0; j < PLAYBACK_ITEMS_N; j++)
	{
		items[j].lgp_id_small = (int)lgp[lgpPlayBackRepeat + j];
		if (j != i)
			items[j].icon = (int*)&icon_unsel;
	}
	
	
	header.lgp_id = (int)lgp[lgpOptionsPlayMode];
	sk[0].lgp_id = (int)lgp[lgpOptionsSelect];
	sk[1].lgp_id = (int)lgp[lgpOptionsBack];
	
	CreateMenu(1, 0, &desc, &header, 0, PLAYBACK_ITEMS_N, NULL, NULL);
}