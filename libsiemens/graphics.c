#include <swilib.h>
#include "graphics.h"
#include "string_utils.h"
#include "obs.h"

IMGHDR *CreateIMGHDRFromImgFile(const char *path)
{
	IMGHDR *img;
	HObj obj;
	short w, h;
	
	obj = CreateHObjFromImgFile(path);
	
	//для jpg обязательно нужны размеры, иначе пикает
	Obs_GetInputImageSize (obj, &w, &h);
	Obs_SetOutputImageSize (obj, w, h);
	
	Obs_Start(obj);
	
	
	img = HObj2IMGHDR(obj);
	Obs_DestroyObject(obj);
	
	return img;
}

void DrawIMGHDR(IMGHDR *img, int x, int y, int x2, int y2, int bleed_x, int bleed_y)
{
	RECT rc;
	DRWOBJ drwobj;
	if (!bleed_x && !bleed_y)
	{
		StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
		SetPropTo_Obj5(&drwobj, &rc, 0, img);
	}
	else
	{
		StoreXYXYtoRECT(&rc, x, y, x2, y2);
		SetProp2ImageOrCanvas(&drwobj, &rc, 0, img, bleed_x, bleed_y);
	}
	SetColor(&drwobj, NULL, NULL);
	DrawObject(&drwobj);
}

void FreeIMGHDR(IMGHDR *img)
{
	if (img)
	{
		if (img->bitmap)
			mfree(img->bitmap);
		mfree(img);
	}
}

//Отрисовывает WS строку, где критичны точные данные отрисовки. Возвращает ширину строки
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//////////////////////только ALIGN_LEFT!!!////////////////////////////////////////////

int DrawStringWS(WSHDR *ws, int x1, int y1, int x2, int y2, int font, int text_attr, const char *pen, const char *brush)
{
	if (text_attr != TEXT_ALIGNLEFT) return 0;
	
	WSHDR *symbol = AllocWS(1);
	symbol->wsbody[0] = 1;
	int ws_width = 0;
	int symbol_width;
	
	for (int i = 0; i < ws->wsbody[0]; i++)
	{
		if (ws_width > (x2 - x1)) break;
		symbol->wsbody[1] = ws->wsbody[1 + i];
		symbol_width = GetSymbolWidth(ws->wsbody[1 + i], font);
		DrawString(symbol, x1 + ws_width, y1, x1 + ws_width + symbol_width, y2, font, text_attr, pen, brush);
		ws_width += symbol_width;
	}
	
	FreeWS(symbol);
	return ws_width;
}

unsigned int GetWidthWS(WSHDR *ws, const int font)
{
	return (Get_WS_width(ws, font) + 1);
}
