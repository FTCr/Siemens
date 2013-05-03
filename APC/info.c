#include <swilib.h>
#include "../libsiemens/graphics.h"
#include "main.h"
#include "info.h"

/*#define ICON_HELP 0x530
#define ICON_INFO 0x531*/

#define HELP  "HotKeys:\n1: Rewind\n3: Forward\n4: Previous track\n5: Toggle\n6: Next track\nGreen: Move cursor to current track\n*: Mute/Unmute\n#: Cut file"
#define ABOUT "APC - client for APD\n\xA9 FIL, 2013"

WSHDR *ws;
int gui_type;

//int hdr_icon;
static HEADER_DESC header = {0, 0, 0, 0, NULL, LGP_NULL, LGP_NULL};

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

static void OnRedrawAbout(void)
{
	int y = YDISP + HeaderH() + (GetIMGHDRFromThemeCache(THEME_CACHE_ID_BODY_TAB)->h - HeaderH()) / 2 - GetFontYSIZE(FONT_MEDIUM);
	DrawString(ws, 0, y, ScreenW(), ScreenH() - SoftkeyH(), FONT_MEDIUM, TEXT_ALIGNMIDDLE + TEXT_OUTLINE, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(1));
}

static int OnKey(GUI *gui, GUI_MSG *msg)
{
	if (msg->keys == 0x0001)
	{
		return 1;
	}
	return 0;
}

static void GHook(GUI *gui, int cmd)
{
	if (cmd == TI_CMD_CREATE)
	{
		if (gui_type == INFO_GUI_TYPE_ABOUT)
		{
			static void *methods[16];
			void **m = GetDataOfItemByID(gui, 4);
			memcpy(methods, m[1], sizeof(methods));
			methods[0] = (void *)OnRedrawAbout;
			m[1] = methods;
		}
	}
	if (cmd == TI_CMD_FOCUS)
	{
		DisableIDLETMR();
	}
	if (cmd == TI_CMD_REDRAW)
	{
		SOFTKEY_DESC sk_left =  {0x0018, 0x0000, (int)""};
		SOFTKEY_DESC sk_right = {0x0001, 0x0000, (int)lgp[lgpOptionsBack]};
		
		SetSoftKey(gui, &sk_left, SET_SOFT_KEY_N);
		SetSoftKey(gui, &sk_right, SET_SOFT_KEY_N == 0 ? 1 : 0);
	}
	if (cmd == TI_CMD_DESTROY)
	{
		FreeWS(ws);
	}
}

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

void CreateInfoGUI(int type)
{
	void *ma = malloc_adr();
	void *eq = AllocEQueue(ma, mfree_adr());
	
	unsigned int len;
	
	EDITCONTROL ec;
	PrepareEditControl(&ec);
	
	gui_type = type;
	if (type == INFO_GUI_TYPE_HELP)
	{
		len = strlen(HELP);
		ws = AllocWS(len);
		str_2ws(ws, HELP, len);
		
		EDITC_OPTIONS ec_options;
		PrepareEditCOptions(&ec_options);
		SetFontToEditCOptions(&ec_options, FONT_SMALL);
		ConstructEditControl(&ec, ECT_READ_ONLY, ECF_APPEND_EOL, ws, len + 1);
		CopyOptionsToEditControl(&ec,&ec_options);
		
		header.lgp_id = (int)lgp[lgpOptionsHelp];
		//hdr_icon = ICON_HELP;
	}
	else
	{
		len = strlen(ABOUT);
		ws = AllocWS(len);
		str_2ws(ws, ABOUT, len);
		//ws->wsbody[22] = 0xA9;
		ConstructEditControl(&ec, ECT_READ_ONLY, ECF_APPEND_EOL, '\0', 1);
		
		header.lgp_id = (int)lgp[lgpOptionsAbout];
		//hdr_icon = ICON_INFO;
	}
	AddEditControlToEditQend(eq, &ec, ma);
	
	patch_header(&header);
	patch_input(&desc);
	CreateInputTextDialog(&desc, &header, eq, 1, NULL);
}
	