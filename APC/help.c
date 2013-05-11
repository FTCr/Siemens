#include <swilib.h>
#include "../libsiemens/graphics.h"
#include "main.h"

#define HELP  "HotKeys:\n1: Rewind\n3: Forward\n4: Previous track\n5: Toggle\n6: Next track\n0: Track info\nGreen: Move cursor to current track\n*: Mute/Unmute\n#: Cut file"

static HEADER_DESC header = {0, 0, 0, 0, NULL, (int)"Help", LGP_NULL};

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

static TVIEW_DESC desc =
{
	8, OnKey, NULL, NULL,
	softkeys,
	&skt,
	{0, 0, 0, 0},
	FONT_SMALL,
	0x64,
	0x65,
	0,
	0
};

void CreateHelpGUI(void)
{
	void *ma = malloc_adr();
	void *mf = mfree_adr();
	
	void *gui = TViewGetGUI(ma, mf);
	TViewSetDefinition(gui, &desc);
	
	static WSHDR *ws;
	int len = strlen(HELP);
	ws = AllocWS(len);
	str_2ws(ws, HELP, len);
	TViewSetText(gui, ws, ma, mf);
	
	patch_header(&header);
	header.lgp_id = (int)lgp[lgpOptionsHelp];
	sk[0].lgp_id  = (int)lgp[lgpInfoOK];
	sk[1].lgp_id  = (int)lgp[lgpInfoBack];
	SetHeaderToMenu(gui, &header, ma);
	patch_input((INPUTDIA_DESC*)&desc);
	CreateGUI(gui);
}