#include <swilib.h>
#include "../../../libsiemens/graphics.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/skin_utils.h"
#include "../../../libshell/other.h"
#include "main.h"
#include "config_data.h"

unsigned int gui_id;

typedef struct{
	GUI gui;
	unsigned int cur_id;
} MAIN_GUI;


static void OnRedraw(MAIN_GUI *data)
{
	DrawBG();
	DrawText(ws, lgp[lgpHeaderReboot + data->cur_id], GetFont(fontHeader1), TEXT_ALIGNMIDDLE, GetColor(colorHeader), TEXT_TYPE_HEADER);
	DrawText(ws, lgp[lgpSoftLeft], GetFont(fontSoftkeys), TEXT_ALIGNLEFT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	DrawText(ws, lgp[lgpSoftRight1], GetFont(fontSoftkeys), TEXT_ALIGNRIGHT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	
	unsigned int space = (ScreenW() - icons_sel[0]->w * 3) / 4;
	for (unsigned int i = 0; i < MAX_ICONS; i++)
	{
		if (data->cur_id == i)
			DrawIMGHDR(icons_sel[i], (i + 1) * space + i * icons_sel[i]->w, cfg_pos_y, 0, 0, 0, 0);
		else
			DrawIMGHDR(icons_unsel[i], (i + 1) * space + i * icons_unsel[i]->w, cfg_pos_y, 0, 0, 0, 0);
	}
}

static void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	data->gui.state = 1;
	data->cur_id    = 1;
}

static void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	gui_id = data->gui.state = 0;
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
			case RIGHT_SOFT: return 1;
			case LEFT_BUTTON:
				if (data->cur_id == 0)
					data->cur_id = MAX_ICONS - 1;
				else
					data->cur_id--;
				DirectRedrawGUI_ID(gui_id);
			break;
			case RIGHT_BUTTON:
				if (data->cur_id == MAX_ICONS - 1)
					data->cur_id = 0;
				else
					data->cur_id++;
				DirectRedrawGUI_ID(gui_id);
			break;
			case LEFT_SOFT: case ENTER_BUTTON:
				switch (data->cur_id)
				{
					case 0:
						RebootPhone();
					break;
					case 1:
						SwitchPhoneOff();
					break;
					case 2:
						GeneralFunc_flag1(gui_id, 0);
						Close();
					break;
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

void CreateTFGUI()
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
