#include <swilib.h>
#include "../libapd/libapd.h"
#include "../libsiemens/graphics.h"

typedef struct
{
	GUI gui;
	WSHDR *ws;
	int id;
} MAIN_GUI;

static RECT Canvas = {0, 0, 0, 0};

static void OnRedraw(MAIN_GUI *data)
{
	RECT rc;
	rc.x = ScreenW() / 10;
	rc.y = ScreenH() / 3 + YDISP;
	rc.x2 = ScreenW() - rc.x;
	rc.y2 = ScreenH() - rc.y;
	DrawRectangle(rc.x, rc.y, rc.x2, rc.y2, 0, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(20));
	
	int x_pos, x2_pos, x_dur, x2_dur, x_pbar_bg, x2_pbar_bg, x2_pbar;
	int y, y2;
	y  = rc.y + (rc.y2 - rc.y) / 2 - GetFontYSIZE(FONT_SMALL) / 2;
	y2 = y + GetFontYSIZE(FONT_SMALL);
	
	TTime time;
	
	GetTimeFromSeconds(&time, APlayer_GetTrackPosition() / 1000);
	wsprintf(data->ws, "%02d:%02d", time.min, time.sec);
	x_pos  = rc.x + 4;
	x2_pos = x_pos + GetWidthWS(data->ws, FONT_SMALL);
	DrawString(data->ws, x_pos, y, x2_pos, y2, FONT_SMALL, TEXT_ALIGNLEFT, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(23));
	
	GetTimeFromSeconds(&time, APlayer_GetTrackDuration() / 1000);
	wsprintf(data->ws, "%02d:%02d", time.min, time.sec);
	x_dur  = rc.x2 - 4 - GetWidthWS(data->ws, FONT_SMALL);
	x2_dur = x_dur + GetWidthWS(data->ws, FONT_SMALL);
	DrawString(data->ws, x_dur, y, x2_dur, y2, FONT_SMALL, TEXT_ALIGNRIGHT, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(23));
	
	x_pbar_bg  = x2_pos + 4;
	x2_pbar_bg = x_dur - 4;
	DrawRectangle(x_pbar_bg, y + 2, x2_pbar_bg, y2 - 2, 0, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(23));
	
	int percent = ProgressCalculate(APlayer_GetTrackPosition(), APlayer_GetTrackDuration());
	x2_pbar = x_pbar_bg + ((float)(percent) / 100) * (x2_pbar_bg - x_pbar_bg);
	DrawRectangle(x_pbar_bg, y + 2, x2_pbar, y2 - 2, 0, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(0));
}


static void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	data->ws = AllocWS(256);
	data->gui.state = 1;
}

static void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	data->gui.state = 0;
	FreeWS(data->ws);
}

static void OnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
	data->gui.state = 2;
}

static void OnUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	if (data->gui.state != 2) return;
	data->gui.state = 1;
}


static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
	const int Key   = msg->gbsmsg->submess;
	const int Style = msg->gbsmsg->msg;
	if (Style == KEY_DOWN || Style == LONG_PRESS)
	{
		switch (Key)
		{
			case '1':
				if (APlayer_GetPlayStatus() != APLAYER_STOP && APlayer_GetPlayStatus() != APLAYER_PAUSE)
				{
					APlayer_Rewind(5);
					DirectRedrawGUI_ID(data->id);
				}
				else return 1;
			break;
			case '3':
				if (APlayer_GetPlayStatus() != APLAYER_STOP && APlayer_GetPlayStatus() != APLAYER_PAUSE)
				{
					APlayer_Forward(5);
					DirectRedrawGUI_ID(data->id);
				}
				else return 1;
			break;
		}
	}
	else if (Style == KEY_UP) return 1;
	return 0;
}


static int method8(void){return(0);}

static int method9(void){return(0);}

extern void kill_data(void *p, void (*func_p)(void *));

const void * const gui_methods[11] = {
  (void *)OnRedraw,	//Redraw
  (void *)OnCreate,	//Create
  (void *)OnClose,	//Close
  (void *)OnFocus,	//Focus
  (void *)OnUnfocus,	//Unfocus
  (void *)OnKey,	//OnKey
  0,
  (void *)kill_data, //method7,	//Destroy
  (void *)method8,
  (void *)method9,
  0
};

void CreateRewindGUI(const int key)
{
	if (APlayer_GetPlayStatus() != APLAYER_STOP)
	{
		MAIN_GUI *main_gui = malloc(sizeof(MAIN_GUI));
		zeromem(main_gui, sizeof(MAIN_GUI));
		Canvas.x2 = ScreenW() - 1;
		Canvas.y2 = ScreenH()-1;
		main_gui->gui.canvas = (void *)(&Canvas);
		main_gui->gui.flag30 = 2;
		main_gui->gui.methods = (void *)gui_methods;
		main_gui->gui.item_ll.data_mfree = (void (*)(void *))mfree_adr();
		main_gui->id = CreateGUI(main_gui);
		GBS_SendMessage(MMI_CEPID, KEY_DOWN, key);
	}
}