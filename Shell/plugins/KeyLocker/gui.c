#include <swilib.h>
#include "../libshell/plugins.h"
#include "main.h"
#include "config_data.h"

unsigned int gui_id;

void *RamScreenBrightness()
__swi(0x80D7);

typedef struct{
	GUI gui;
	int max_illum;
} MAIN_GUI;

static void OnRedraw(MAIN_GUI *data)
{
	DrawRectangle(0, 0, ScreenW(), ScreenH(), 0, GetPaletteAdrByColorIndex(1), GetPaletteAdrByColorIndex(1));
}

static void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	data->gui.state = 1;
	
	LIGHT_PARAM *lp = RamScreenBrightness();
	data->max_illum = lp->timeout;
	SetIllumination(0, 1, 0, 0);
	SetIllumination(1, 1, 0, 0);
	SaveMaxIllumination(0);
}

static void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	gui_id = data->gui.state = 0;
#ifdef ELKA
	DisableIconBar(0);
	GBS_SendMessage(0x4209, 0x642C, 0, 0, 0);
#endif
	SetIllumination(0, 1, data->max_illum, 0);
	SetIllumination(1, 1, data->max_illum, 0);
	SaveMaxIllumination(data->max_illum);
}

static void OnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
	data->gui.state = 2;
#ifdef ELKA
	DisableIconBar(1);
#endif
}

static void OnUnFocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	if (data->gui.state != 2) return;
	data->gui.state = 1;
#ifdef ELKA
	DisableIconBar(0);
	GBS_SendMessage(0x4209, 0x642C, 0, 0, 0);
#endif
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
	const unsigned int Key  = msg->gbsmsg->submess;
	const unsigned int Type = msg->gbsmsg->msg;
	
	if (Key == cfg_key)
	{
		switch (cfg_type_block)
		{
			case CFG_KEY_DOWN:
				if (Type == KEY_DOWN)
				{
					KbdUnlock();
				}
			break;
			case CFG_LONG_PRESS:
				if (Type == LONG_PRESS)
				{
					KbdUnlock();
				}
			break;
			case CFG_DOUBLE_CLICK:
				if (Type == KEY_DOWN)
				{
					if (!DLOCK)
					{
						DLOCK = 1;
						GBS_StartTimerProc(&tmr, 50, (void*)DLock);
					}
					else if (DLOCK == 1)
					{
						KbdUnlock();
					}
				}
			break;
		}
	}
	
	return 0;
}

static int Method8(void){return 0;}
static int Method9(void){return 0;}

extern void kill_data(void *p, void (*func_p)(void *));

static const void * const gui_methods[11]=
{
	(void*)OnRedraw,
	(void*)OnCreate,
	(void*)OnClose,
	(void*)OnFocus,
	(void*)OnUnFocus,
	(void*)OnKey,
	0,
	(void*)kill_data,
	(void*)Method8,
	(void*)Method9,
	0
};

void CreateSSGUI(void)
{	
	static RECT Canvas = {0,0,0,0};
	MAIN_GUI *main_gui = malloc(sizeof(MAIN_GUI));
	zeromem(main_gui,sizeof(MAIN_GUI));
	Canvas.x2 = ScreenW() - 1;
	Canvas.y2 = ScreenH() - 1;
	main_gui->gui.canvas             = (RECT *)(&Canvas);
	main_gui->gui.methods            = (void *)gui_methods;
	main_gui->gui.item_ll.data_mfree = (void (*)(void *))mfree_adr();
	gui_id = CreateGUI(main_gui);
}

void CloseSSGUI(void)
{
	if (gui_id)
		GeneralFunc_flag1(gui_id, 0);
}
