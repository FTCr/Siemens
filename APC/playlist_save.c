#include <swilib.h>
#include <libapd.h>
#include "main.h"
#include "../libsiemens/graphics.h"

static HEADER_DESC header = {0, 0, 0, 0, NULL, LGP_NULL, LGP_NULL};

static int OnKey(GUI *gui, GUI_MSG *msg)
{
	if (msg->keys == 0x018)
	{
		EDITCONTROL ec;
		ExtractEditControl(gui, EDIT_GetFocus(gui), &ec);
		char name[128];
		ws_2str(ec.pWS, name, ec.pWS->wsbody[0]);
		APlayer_SavePlayList(name);
		GeneralFuncF1(2);
	}
	return 0;
}

static void GHook(GUI *gui, int cmd)
{
	if (cmd == TI_CMD_REDRAW)
	{
		SOFTKEY_DESC sk = {0x0018, 0x0000, (int)lgp[lgpPlaylistSave]};
		SetSoftKey(gui, &sk, SET_SOFT_KEY_N);
	}
}

extern int softkeys[];

static SOFTKEY_DESC sk[]=
{
	{0x0018, 0x0000, (int)""},
	{0x0001, 0x0000, (int)"Back"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB skt =
{
	sk, 0
};

static INPUTDIA_DESC desc =
{
	1, OnKey, GHook, NULL,
	0,
	&skt,
	{0, 0, 0, 0},
	FONT_SMALL,
	100,
	101,
	0,
	0,
	0x40000000
};

void CreatePlaylistSaveGUI(void)
{	
	void *ma = malloc_adr();
	void *eq = AllocEQueue(ma, mfree_adr());
	WSHDR *ws = AllocWS(128 - 5);
	
	EDITCONTROL ec;
	PrepareEditControl(&ec);
	
	wsprintf(ws, "%t", lgp[lgpPlaylistSaveHead]);
	ConstructEditControl(&ec, ECT_HEADER, ECF_APPEND_EOL, ws, ws->wsbody[0]);
	AddEditControlToEditQend(eq, &ec, ma);
	wsprintf(ws,  "playlist");
	ConstructEditControl(&ec, ECT_NORMAL_TEXT, ECF_APPEND_EOL, ws, ws->wsbody[0] + 5);
	AddEditControlToEditQend(eq, &ec, ma);
	
	patch_header(&header);
	patch_input(&desc);
	
	sk[1].lgp_id = (int)lgp[lgpOptionsBack];
	header.lgp_id = (int)lgp[lgpOptionsPlaylistSave];
	
	CreateInputTextDialog(&desc, &header, eq, 1, NULL);
	
	FreeWS(ws);
}