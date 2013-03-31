#include <swilib.h>
#include "../libshell/plugins.h"
#include "main.h"
#include "config_data.h"

unsigned int gui_id;

typedef struct{
	GUI gui;
	int illum_level;
} MAIN_GUI;

static void OnRedraw(MAIN_GUI *data)
{
	DrawRectangle(0, 0, ScreenW(), ScreenH(), 0, GetPaletteAdrByColorIndex(1), GetPaletteAdrByColorIndex(1));
}

void IllumHook(int mode, int *save_illum_level)
{
	if (mode == 0) //вырубаем подсветку
	{
		LIGHT_PARAM *lp = RamScreenBrightness();
		*save_illum_level = lp->max_illum;
		SetIllumination(0, 1, 0, 0);
		SetIllumination(1, 1, 0, 0);
		SaveMaxIllumination(0);
		IllumTimeRequest(4,3);
	}
	else
	{
		SetIllumination(0, 1, *save_illum_level, 0);
		SetIllumination(1, 1, *save_illum_level, 0);
		SaveMaxIllumination(*save_illum_level);
		IllumTimeRelease(4,3);
	}
}

static void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	data->gui.state = 1;
}

static void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	gui_id = data->gui.state = 0;
#ifdef ELKA
	DisableIconBar(0);
	GBS_SendMessage(MMI_CEPID, MSG_REFRESH_ICONBAR);
#endif
	IllumHook(1, &data->illum_level);
}

static void OnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
	data->gui.state = 2;
#ifdef ELKA
	DisableIconBar(1);
#endif
	IllumHook(0, &data->illum_level);
}

static void OnUnFocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	if (data->gui.state != 2) return;
	data->gui.state = 1;
#ifdef ELKA
	DisableIconBar(0);
	GBS_SendMessage(MMI_CEPID, MSG_REFRESH_ICONBAR);
#endif
	IllumHook(1, &data->illum_level);
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
						GBS_StartTimerProc(&tmr, cfg_wait_time, (void*)DLock);
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
