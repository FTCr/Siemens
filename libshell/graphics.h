#ifndef _LIBSHELL_GRAPHICS_H_
	#define _LIBSHELL_GRAPHICS_H_
	
#define TEXT_TYPE_HEADER  0
#define TEXT_TYPE_SOFTKEY 1

enum
{
	imgWallpaper,
	imgBottom,
	imgHeader,
	imgCursor,
	imgTotal
};

extern IMGHDR *img[imgTotal];

int LoadGraphics(void);
void UploadGraphics(void);

void DrawText(WSHDR *ws, const char *text, const int font, const int align, const char *color, const int type);
void DrawSeparateBG(const int x, const int y, const int x2, const int y2);
void DrawBG(void);

#endif
