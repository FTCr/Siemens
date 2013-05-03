#include <swilib.h>
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

void DrawIMGHDR(IMGHDR *img, int x, int y, int w, int h)
{
	RECT rc;
	DRWOBJ drwobj;
	StoreXYWHtoRECT(&rc, x, y, w, h);
	SetPropTo_Obj5(&drwobj, &rc, 0, img);
	SetColor(&drwobj, NULL, NULL);
	DrawObject(&drwobj);
}

void DrawIMGHDR_2(IMGHDR *img, int x, int y, int x2, int y2, int bleed_x, int bleed_y)
{
	RECT rc;
	DRWOBJ drwobj;
	StoreXYXYtoRECT(&rc, x, y, x2, y2);
	SetProp2ImageOrCanvas(&drwobj, &rc, 0, img, bleed_x, bleed_y);
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

unsigned int GetWidthWS(WSHDR *ws, const int font)
{
	return (Get_WS_width(ws, font) + 1);
}

void patch_rect(RECT*rc, int x, int y, int x2, int y2)
{
	rc->x  = x;
	rc->y  = y;
	rc->x2 = x2;
	rc->y2 = y2;
}

void patch_header(HEADER_DESC* head)
{
	head->rc.x  = 0;
	head->rc.y  = YDISP;
	head->rc.x2 = ScreenW() - 1;
	head->rc.y2 = HeaderH() + YDISP - 1;
}

void patch_header_small(HEADER_DESC* head)
{
	head->rc.x  = 3;
	head->rc.x2 = ScreenW()-6;
	#ifndef ELKA
		head->rc.y  = YDISP + 0x18;
		head->rc.y2 = YDISP + 0x18 + 0x13;
	#else
		head->rc.y  = YDISP + 0x23;
		head->rc.y2 = YDISP + 0x23+0x22;
	#endif
}

void patch_input(INPUTDIA_DESC* inp)
{
	inp->rc.x  = 0;
	inp->rc.y  = HeaderH() + 1 + YDISP;
	inp->rc.x2 = ScreenW() - 1;
	inp->rc.y2 = ScreenH() - SoftkeyH() - 1;
}