#include <swilib.h>
#include <libid3.h>
#include "../libsiemens/graphics.h"
#include "main.h"

static HEADER_DESC header = {0, 0, 0, 0, NULL, (int)"Track info", LGP_NULL};

extern int softkeys[];

static int OnKey(GUI *gui, GUI_MSG *msg)
{
	if (msg->keys == 0x3D || msg->keys == 0x01)
		return 1;
	return 0;
}

static void GHook(GUI *gui, int cmd)
{
	if (cmd == TI_CMD_REDRAW)
	{
		static SOFTKEY_DESC sk_m = {0x003D, 0x0000, (int)LGP_DOIT_PIC};
		static SOFTKEY_DESC sk_r = {0x0001, 0x0000, LGP_NULL};
		sk_r.lgp_id = (int)lgp[lgpInfoBack];
		SetSoftKey(gui, &sk_m, SET_SOFT_KEY_M);
		SetSoftKey(gui, &sk_r, SET_SOFT_KEY_N == 0 ? 1 : 0);
	}
}

static SOFTKEY_DESC sk[]=
{
	{0x0018, 0x0000, (int)"OK"},
	{0x0001, 0x0000, LGP_NULL},
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
	FONT_MEDIUM,
	100,
	101,
	0,
	0,
	0x40000000
};

void CreateTagsGUI(ID3 *id3)
{	
	void *ma = malloc_adr();
	void *eq = AllocEQueue(ma, mfree_adr());
	EDITCONTROL ec;
	PrepareEditControl(&ec);
	
	WSHDR *ws = AllocWS(256);
	for (int i = 0; i < ID3_ARRAY_SIZE; i++)
	{
		wsprintf(ws, "%t", lgp[lgpTagTitle + i]);
		ConstructEditControl(&ec, ECT_HEADER, ECF_APPEND_EOL, ws, ws->wsbody[0] + 1);
		AddEditControlToEditQend(eq, &ec, ma);
		if (id3->tag[i])
		{
			ConstructEditControl(&ec, ECT_READ_ONLY, ECF_APPEND_EOL, id3->tag[i], id3->tag[i]->wsbody[0] +1);
			AddEditControlToEditQend(eq, &ec, ma);
		}
		else
		{
			wsprintf(ws, "");
			ConstructEditControl(&ec, ECT_READ_ONLY, ECF_APPEND_EOL, ws, 1);
			AddEditControlToEditQend(eq, &ec, ma);
		}
	}
	FreeWS(ws);
	
	patch_header(&header);
	patch_input(&desc);
	
	sk[0].lgp_id  = (int)lgp[lgpInfoOK];
	header.lgp_id = (int)lgp[lgpOptionsTrackInfo];
	
	CreateInputTextDialog(&desc, &header, eq, 1, NULL);
}
