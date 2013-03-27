#include <swilib.h>
#include "../../../libsiemens/graphics.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/skin_utils.h"
#include "config_data.h"

typedef struct{
	GUI gui;
	WSHDR *ws;
} MAIN_GUI;

enum
{
	lgpCallHeader,
	lgpCallSoftLeft,
	lgpCallSoftRight1,
	lgpCallSoftRight2
};

char **lgp;

unsigned int gui_id;

char *number;
unsigned int sym_on_line;
unsigned int lines;
unsigned int dig_id;

static void OnRedraw(MAIN_GUI *data)
{
	DrawBG();

	DrawText(data->ws, lgp[lgpCallHeader], GetFont(fontHeader1), TEXT_ALIGNMIDDLE, GetColor(colorHeader), TEXT_TYPE_HEADER);
	DrawText(data->ws, lgp[lgpCallSoftLeft], GetFont(fontSoftkeys), TEXT_ALIGNLEFT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	if (dig_id != 1)
		DrawText(data->ws, lgp[lgpCallSoftRight2], GetFont(fontSoftkeys), TEXT_ALIGNRIGHT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	else
		DrawText(data->ws, lgp[lgpCallSoftRight1], GetFont(fontSoftkeys), TEXT_ALIGNRIGHT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);

	//набор номера
	if (number)
	{
		unsigned int font = GetFont(fontMain3);
		unsigned int y;
		wsprintf(data->ws, "%s", number);
		y = ICONBAR_H + img[imgHeader]->h + (ScreenH() - (ICONBAR_H + img[imgHeader]->h + img[imgBottom]->h))/2
			- GetFontYSIZE(font) / 2 - (GetFontYSIZE(font) /2 * lines);
			DrawString(data->ws, 0, y, ScreenW(), ScreenH(), font, TEXT_ALIGNLEFT, GetColor(colorMain), GetPaletteAdrByColorIndex(23));
	}
}

static void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	data->gui.state = 1;
	data->ws = AllocWS(128);
}

static void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	gui_id = data->gui.state = 0;
	FreeWS(data->ws);
	mfree(number);
	number = NULL;
	dig_id = 0;
	sym_on_line = 0;
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

void AddDigit(const char symbol)
{	
	number = realloc(number, dig_id + 2);
	number[dig_id] = symbol;
	number[dig_id + 1] = '\0';
	dig_id++;
}

void DelDigit(void)
{
	dig_id--;
	number = realloc(number, dig_id + 1);
	if (!dig_id)
	{
		number = NULL;
		return;
	}
	number[dig_id] = '\0';
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
	const unsigned int Key  = msg->gbsmsg->submess;
	const unsigned int Type = msg->gbsmsg->msg;
	char symbol;

	switch (Type)
	{
		case KEY_DOWN:
			if (Key >= '0' && Key <= '9')
			{
				symbol = Key - '\0';
				goto ADD_DIGIT;
			}
			else if (Key == '*' || Key == '#')
			{
				symbol = Key;
				ADD_DIGIT:
					if (dig_id != sym_on_line * cfg_max_num_lines)
					{
						AddDigit(symbol);
						if (dig_id % sym_on_line == 0 && lines < cfg_max_num_lines - 1)
							lines++;
						DirectRedrawGUI_ID(gui_id);
					}
			}
			else if (Key == GREEN_BUTTON || Key == LEFT_SOFT)
			{
				MakeVoiceCall(number, 0x10, 0x2FFF);
				return 1;
			}
			else if (Key == RIGHT_SOFT)
				goto DEL_DIGIT;
		break;
		case LONG_PRESS:
			switch (Key)
			{
				case RIGHT_SOFT:
					DEL_DIGIT:
						if (dig_id > 1)
						{
							if (dig_id % sym_on_line == 0 && dig_id != sym_on_line * cfg_max_num_lines)
								lines--;
							DelDigit();
							DirectRedrawGUI_ID(gui_id);
						}
						else
						{
							DelDigit();
							return 1;
						}
				break;
				case '#':
					number[dig_id - 1] = '?';
					DirectRedrawGUI_ID(gui_id);
				break;
			}
		break;
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

void CreateDialGUI(const char symbol)
{
	static RECT Canvas = {0,0,0,0};
	MAIN_GUI *main_gui = malloc(sizeof(MAIN_GUI));
	zeromem(main_gui,sizeof(MAIN_GUI));
	Canvas.x2 = ScreenW() - 1;
	Canvas.y2 = ScreenH() - 1;
	main_gui->gui.canvas             = (RECT *)(&Canvas);
	main_gui->gui.methods            = (void *)gui_methods;
	main_gui->gui.item_ll.data_mfree = (void (*)(void *))mfree_adr();
	AddDigit(symbol);
	
	//считаем количество макс. символом для набора номера
	WSHDR *ws = AllocWS(12);
	wsAppendChar(ws, '0');
	const unsigned int Swidth = GetWidthWS(ws, GetFont(fontMain3));
	unsigned int width = 0;
	while((int)width < ScreenW())
	{
		width+=Swidth;
		sym_on_line++;
	}
	
	
	gui_id = CreateGUI(main_gui);
}
