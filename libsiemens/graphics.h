#ifndef _LIBSIEMENS_GRAPHICS_H_
	#define _LIBSIEMENS_GRAPHICS_H_

//создание из любого графического файла
IMGHDR *CreateIMGHDRFromImgFile(const char *path);
//отрисовка
void DrawIMGHDR(IMGHDR *img, int x, int y, int w, int h);
//отрисовка по координатам
void DrawIMGHDR_2(IMGHDR *img, int x, int y, int x2, int y2, int bleed_x, int bleed_y);
//очистка
void FreeIMGHDR(IMGHDR *img);
//получение ширины строки
unsigned int GetWidthWS(WSHDR *ws, const int font);

void patch_rect(RECT*rc, int x, int y, int x2, int y2);
void patch_header(HEADER_DESC* head);
void patch_header_small(HEADER_DESC* head);
void patch_input(INPUTDIA_DESC* inp);
#endif
