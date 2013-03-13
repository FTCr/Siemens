#include <swilib.h>
#include "../../plg.h"
#include "../../libbasefunc/basefunc.h"
#include "../../call_back.h"

#define ID  1

WSHDR *ws;
CALL_BACK *func4s;

void OnRedraw(void)
{
	func4s->DrawText(ws, "Inbox", FONT_SMALL_BOLD, TEXT_ALIGNLEFT, GetPaletteAdrByColorIndex(0), STRING_TYPE_SOFTKEY);
}

void Create(void)
{
	ws=AllocWS(16);
}

void OnKey(int key, int type)
{
	if (type == KEY_DOWN)
	{
		if (key == LEFT_SOFT)  RunShortcut("DEVICE_INBOX");
		if (key == GREEN_BUTTON) RunShortcut("REC_DIALED_LIST");
	}
}

void Destroy(void)
{
	FreeWS(ws);
}

void _init(PLUGIN_S4T *plg, CALL_BACK *cb){
	plg->OnRedraw = (void*)OnRedraw;
	plg->Create   = (void*)Create;
	plg->OnKey    = (void(*)(int,int))OnKey;
	plg->Destroy  = (void*)Destroy;

	plg->id = ID;

	func4s=cb;
}
