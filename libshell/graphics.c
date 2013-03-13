#include <swilib.h>
#include "../libsiemens/graphics.h"
#include "../libsiemens/cfg.h"
#include "../libsiemens/files.h"
#include "../libsiemens/obs.h"
#include "graphics.h"
#include "other.h"
#include "config_coord.h"

IMGHDR *img[imgTotal];


char wallpaper_path[128];

int LoadGraphics(void)
{	
	FSTATS fs;
	char path[256];
	unsigned int err;
	unsigned int i = 1;

	const char *Names[] = {"bottom", "header", "cursor"};
	while(i < imgTotal)
	{
		sprintf(path, "%s%s%s", img_dir, Names[i - 1], ".png");
		if (GetFileStats(path, &fs, &err) == -1)
		{
			sprintf(path, "%s%s%s", Names[i - 1], ".png ", "not found!");
			MsgBoxError(1, (int)path);
			return -1;
		}
		img[i++] = CreateIMGHDRFromPngFile(path, 0);
	}
	
	if (GetStrDataFromSystemConf(wallpaper_path, PROFILE_PD, KEYWORD_WALLPAPER)  == -1)
	{
		if (GetStrDataFromSystemConf(wallpaper_path, PROFILE_PD, KEYWORD_WALLPAPER2) == 1)
		{
			MsgBoxError(1, (int)"Error parse profile.pd!");
			return -1;
		}
	}
	utf8_2fname(wallpaper_path, wallpaper_path);
	
	if (GetFileStats(wallpaper_path, &fs, &err) == -1)
	{
		MsgBoxError(1, (int)"Wallpaper not found!");
		return -1;
	}

	img[imgWallpaper] = CreateIMGHDRFromImgFile(wallpaper_path);
	
	return 0;
}

void UploadGraphics(void)
{
	for (int i = 0; i < imgTotal; i++)
		FreeIMGHDR(img[i]);
}

RECT *GetTextCoord(WSHDR *ws, const unsigned int y, const unsigned int offset_x, const int font, const int align)
{
	RECT *rc = malloc(sizeof(RECT));
	switch (align)
	{
		case TEXT_ALIGNLEFT:
			rc->x  = offset_x;
			rc->x2 = offset_x + GetWsWidth(ws, font);
		break;
		case TEXT_ALIGNMIDDLE:
			rc->x  = 0;
			rc->x2 = ScreenW();
		break;
		case TEXT_ALIGNRIGHT:
			rc->x  = ScreenW() - GetWsWidth(ws, font) - offset_x;
			rc->x2 = rc->x + GetWsWidth(ws, font);
		break;
	}
	rc->y  = y;
	rc->y2 = rc->y + GetFontYSIZE(font);
	return rc;
}

void DrawText(WSHDR *ws, const char *text, const int font, const int align, const char *color, const int type)
{
	RECT *rc;
	wsprintf(ws, "%t", text);
	if (type == TEXT_TYPE_HEADER)
		rc = GetTextCoord(ws, ICONBAR_H + (img[imgHeader]->h - GetFontYSIZE(font)) / 2, cfg_coord_head_off_x, font, align);
	else
		rc = GetTextCoord(ws, (ScreenH() - img[imgBottom]->h) + (img[imgBottom]->h - GetFontYSIZE(font)) / 2, cfg_coord_soft_off_x,
			font, align);
	DrawString(ws, rc->x, rc->y, rc->x2, rc->y2, font, align, color, GetPaletteAdrByColorIndex(23));
	mfree(rc);
}

void DrawSeparateBG(const int x, const int y, const int x2, const int y2)
{
	DrawIMGHDR(img[imgWallpaper], x, y, x2, y2, x, y-ICONBAR_H);
}

void DrawBG(void)
{
	unsigned int x = ICONBAR_H;
	DrawIMGHDR(img[imgWallpaper], 0, ICONBAR_H, 0, 0, 0, 0);
	if(img[imgBottom]) DrawIMGHDR(img[imgBottom], 0, ScreenH() - img[imgBottom]->h, 0, 0, 0, 0);
	DrawIMGHDR(img[imgHeader], 0, ICONBAR_H, 0, 0, 0, 0);
}
