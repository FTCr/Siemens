#include <swilib.h>
#include "../../../libsiemens/graphics.h"
#include "../../../libsiemens/other.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/skin_utils.h"
#include "../../../libshell/menu.h"
#include "../../../libshell/config_coord.c"
#include "main.h"
#include "config_data.h"

#define TOTAL_PROFILES 0x08

MENU *menu;
WSHDR *ws1[TOTAL_PROFILES], *ws2[TOTAL_PROFILES];

unsigned int gui_id;

typedef struct{
	GUI gui;
	WSHDR *ws;
} MAIN_GUI;


static void OnRedraw(MAIN_GUI *data)
{
	DrawBG();
	//заголовок
	DrawText(data->ws, lgp[lgpHeader], GetFont(fontHeader1), TEXT_ALIGNMIDDLE, GetColor(colorHeader), TEXT_TYPE_HEADER);
	
	DrawText(data->ws, lgp[lgpSoftLeft], GetFont(fontSoftkeys), TEXT_ALIGNLEFT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	DrawText(data->ws, lgp[lgpSoftRight], GetFont(fontSoftkeys), TEXT_ALIGNRIGHT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	DrawSMenu(menu);
}

static void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	data->gui.state = 1;
	data->ws = AllocWS(128);
	//загрузка данных для меню
	for (int i = 0; i < TOTAL_PROFILES; i++)
	{
		//имена профилей
		ws1[i] = AllocWS(32);
		if (GetProfile() == i)
		{
			getProfileNameByN(data->ws, i, 0);
			//wsAppendChar(ws1[i], '[');
			wsprintf(ws1[i], "[%w]", data->ws);
			
		}
		else
		{
			getProfileNameByN(ws1[i], i, 0);
		}
		//подсказки
		ws2[i] = AllocWS(128);
		wsprintf(ws2[i], "%t", lgp[lgpMenu_add1 + i]);
	}
	menu = CreateSMenu(ws1, ws2, icon, MENU_ONE_ICONS, NULL, ICONBAR_H + img[imgHeader]->h + cfg_coord_menu2_off_y,
		cfg_coord_max_menu2_items, TOTAL_PROFILES);
}

static void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	gui_id = data->gui.state = 0;
	FreeWS(data->ws);
	for (int i = 0; i < TOTAL_PROFILES; i++)
	{
		FreeWS(ws1[i]);
		FreeWS(ws2[i]);
	}
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
			case LEFT_SOFT: case ENTER_BUTTON:
				SetProfile(menu->cur_id);
				return 1;
			break;
			case RIGHT_SOFT:
				return 1;
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

void CreatePrGUI(void)
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
