#ifndef _LIBS_GRAPHICS_H_
	#define _LIBS_GRAPHICS_H_

#ifdef ELKA
	#define ICONBAR_H 24
#else
	#define ICONBAR_H 0
#endif

#define THEME_CACHE_ID_HEADLINE_STANDARD        0x00
#define THEME_CACHE_ID_HEADLINE_FULLSCREEN      0x01
#define THEME_CACHE_ID_BODY_STANDARD            0x02
#define THEME_CACHE_ID_BODY_TAB                 0x03
#define THEME_CACHE_ID_BOTTOM_STANDARD          0x04
#define THEME_CACHE_ID_BOTTOM_FULLSCREEN        0x05
#define THEME_CACHE_ID_POPUP_OPTIONS            0x06
#define THEME_CACHE_ID_POPUP_FEEDBACK           0x07
#define THEME_CACHE_ID_SELECTION_1LINE          0x08
#define THEME_CACHE_ID_SELECTION_2LINE          0x09
#define THEME_CACHE_ID_SELECTION_3LINE          0x0A
#define THEME_CACHE_ID_SELECTION_ICON           0x0B
#define THEME_CACHE_ID_POPUP_SEARCH_LINE        0x0C
#define THEME_CACHE_ID_POPUP_QUICK_ACCESS_FIELD 0x0D
#define THEME_CACHE_ID_PROGRESS_STATUSBAR       0x0E
#define THEME_CACHE_ID_TAB_SELECTED             0x0F
#define THEME_CACHE_ID_TAB_UNSELECTED           0x10
#define THEME_CACHE_ID_STATUSBAR_STANDARD       0x11
#define THEME_CACHE_ID_STATUSBAR_FULLSCREEN     0x12


IMGHDR *CreateIMGHDRFromImgFile(const char *path);
void DrawIMGHDR(IMGHDR *img, int x, int y, int x2, int y2, int bleed_x, int bleed_y);
void FreeIMGHDR(IMGHDR *img);

int DrawStringWS(WSHDR *ws, int x1, int y1, int x2, int y2, int font, int text_attr, const char *pen, const char *brush);
unsigned int GetWidthWS(WSHDR *ws, const int font);	
#endif
