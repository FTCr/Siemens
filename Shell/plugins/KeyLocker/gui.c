#include <swilib.h>
#include "../libshell/plugins.h"
#include "../libshell/other.h"
#include "main.h"
#include "config_data.h"

void *RamScreenBrightness()
__swi(0x80D7);

typedef struct{
	GUI gui;
} MAIN_GUI;

static void OnRedraw(MAIN_GUI *data)
{
	DrawRectangle(0, 0, ScreenW(), ScreenH(), 0, GetPaletteAdrByColorIndex(1), GetPaletteAdrByColorIndex(1));
}

static void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	data->gui.state = 1;
}

static void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	keyblock_id = data->gui.state = 0;
#ifdef ELKA
	DisableIconBar(0);
#endif
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
#endif
}

void UnLock()
{
	KbdUnlock();
	LIGHT_PARAM *lp;
	lp = (LIGHT_PARAM*)RamScreenBrightness();
	SetIllumination(0, 1, lp->unk0, 0); //в swlib.h перепутаны поля структуры
	SetIllumination(1, 1, lp->unk0, 0);
	//запуск автообновления плагинов
	if (plg)
	{
		for (int i = 0; plg[i] != NULL; i++)
			if (IsUsePlg(plg[i]) && plg[i]->StartUpdate) plg[i]->StartUpdate();
	}
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
					UnLock();
					return 1;
				}
			break;
			case CFG_LONG_PRESS:
				if (Type == LONG_PRESS)
				{
					UnLock();
					return 1;
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
						UnLock();
						return 1;
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

void CreateSSGUI()
{
	static RECT Canvas = {0,0,0,0};
	MAIN_GUI *main_gui = malloc(sizeof(MAIN_GUI));
	zeromem(main_gui,sizeof(MAIN_GUI));
	Canvas.x2 = ScreenW() - 1;
	Canvas.y2 = ScreenH() - 1;
	main_gui->gui.canvas             = (RECT *)(&Canvas);
	main_gui->gui.methods            = (void *)gui_methods;
	main_gui->gui.item_ll.data_mfree = (void (*)(void *))mfree_adr();
	CreateGUI(main_gui);
}
