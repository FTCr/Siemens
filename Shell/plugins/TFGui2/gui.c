#include <swilib.h>
#include "../../../libsiemens/graphics.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/skin_utils.h"
#include "../../../libshell/other.h"
#include "../../../libshell/menu.h"
#include "../../../libshell/config_coord.h"
#include "main.h"
#include "config_data.h"

typedef struct{
	GUI gui;
} MAIN_GUI;

unsigned int gui_id;

MENU *menu;

void shutdown_func(void){SwitchPhoneOff();}
void reboot_func(void){RebootPhone();}

void close_func(void)
{
	Close();
}
void reload_func(void)
{
	Reload();
}

void kbdlock_func(void)
{
	KbdLock();
}

void *menu_procs[MENU_ITEMS]=
{
	shutdown_func,
	reboot_func,
	close_func,
	reload_func,
	kbdlock_func
};

static void OnRedraw(MAIN_GUI *data)
{
	DrawBG();
	DrawText(ws, lgp[lgpHeader], GetFont(fontHeader1), TEXT_ALIGNMIDDLE, GetColor(colorHeader), TEXT_TYPE_HEADER);
	DrawText(ws, lgp[lgpSoftLeft], GetFont(fontSoftkeys), TEXT_ALIGNLEFT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	DrawText(ws, lgp[lgpSoftRight], GetFont(fontSoftkeys), TEXT_ALIGNRIGHT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	DrawSMenu(menu);
}

static void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	menu = CreateSMenu(strings1, strings2, MENU_ENC_CP1251, icons, MENU_MANY_ICONS, menu_procs, ICONBAR_H + img[imgHeader]->h + cfg_coord_menu2_off_y,
			cfg_coord_max_menu2_items, MENU_ITEMS);
	data->gui.state = 1;
}

static void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	gui_id = data->gui.state = 0;
	DestroySMenu(menu);
}

static void OnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
	data->gui.state = 2;
	DisableIDLETMR();
}

static void OnUnFocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	if (data->gui.state != 2) return;
	data->gui.state = 1;
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
	const unsigned int Key  = msg->gbsmsg->submess;
	const unsigned int Type = msg->gbsmsg->msg;
	if (Type == KEY_DOWN || Type == LONG_PRESS)
	{
		switch (Key)
		{
			case UP_BUTTON:
				OnKeySMenu(menu, MENU_UP);
			break;
			case DOWN_BUTTON:
				OnKeySMenu(menu, MENU_DOWN);
			break;
			case ENTER_BUTTON: case LEFT_SOFT:
				ActionSMenu(menu);
				return 1;
			break;
			case RIGHT_SOFT:
				return 1;
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

void CreateTFGUI2(void)
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
