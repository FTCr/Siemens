#include <swilib.h>
#include <libaudio.h>
#include "../libsiemens/graphics.h"
#include "main.h"

static HEADER_DESC header = {0, 0, 0, 0, NULL, (int)"test", LGP_NULL};

extern int softkeys[];

static SOFTKEY_DESC sk[]=
{
	{0x0018, 0x0000, (int)"OK"},
	{0x0001, 0x0000, (int)"Exit"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB skt =
{
	sk, 0
};

static INPUTDIA_DESC desc =
{
	1, NULL, NULL, NULL,
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

/*unsigned int CreateTrackInfoGUI(A_TAG *tag)
{	
	unsigned int ret = 0;
	
	void *ma = malloc_adr();
	void *eq = AllocEQueue(ma, mfree_adr());
	WSHDR *ws = AllocWS(256);
	
	EDITCONTROL ec;
	PrepareEditControl(&ec);
	
	for (int i = 0; i < A_TAG_MAX; i++)
	{
		if (tag->data[i]->wsbody[0]) ret = 1;
		wsprintf(ws, "%t", lgp[lgpTrackInfoTitle + i]);
		ConstructEditControl(&ec, ECT_HEADER, ECF_APPEND_EOL, ws, ws->wsbody[0] + 1);
		AddEditControlToEditQend(eq, &ec, ma);
		ConstructEditControl(&ec, ECT_NORMAL_TEXT, ECF_APPEND_EOL, tag->data[i], tag->data[i]->wsbody[0] +1);
		AddEditControlToEditQend(eq, &ec, ma);
	}
	
	patch_header(&header);
	patch_input(&desc);
	
	header.lgp_id = (int)lgp[lgpOptionsTrackInfo];
	
	CreateInputTextDialog(&desc, &header, eq, 1, NULL);
	
	FreeWS(ws);
	
	return ret;
}*/
