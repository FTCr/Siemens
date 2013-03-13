#ifndef _LIBSHELL_GRAPHICS_H_
	#define _LIBSHELL_GRAPHICS_H_
	
#define TEXT_TYPE_HEADER  0
#define TEXT_TYPE_SOFTKEY 1

#define PROFILE_PD         "1:\\system\\hmi\\profile.pd"
#define KEYWORD_WALLPAPER  "Wallpaper_3"
#define KEYWORD_WALLPAPER2 "Wallpaper_1"

enum
{
	imgWallpaper,
	imgBottom,
	imgHeader,
	imgCursor,
	imgTotal
};

extern IMGHDR *img[imgTotal];

extern char wallpaper_path[128];

int LoadGraphics(void);
void UploadGraphics(void);

void DrawText(WSHDR *ws_allocate, const char *text, const int font, const int align, const char *color, const int type);
void DrawSeparateBG(const int x, const int y, const int x2, const int y2);
void DrawBG(void);

#endif
