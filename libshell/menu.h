#ifndef _LIBSHELL_MENU_H_
	#define _LIBSHELL_MENU_H_

#define MENU_UP   0x00
#define MENU_DOWN 0x01

#define MENU_ONE_ICONS  0x00
#define MENU_MANY_ICONS 0x01

typedef struct
{
	WSHDR *ws1;
	WSHDR *ws2;
	IMGHDR *icon;
	void *proc;
}MENU_ITEM;

typedef struct
{
	MENU_ITEM **items;
	
	int y;
	int first_item; //первый пункт для отрисовки
	int max;        //количество пунктов для отрисовки
	int total;      //общее число пунктов
	
	int data;       //вспомогательная переменная для перемещения курсора
	int cur_id;     //позиция курсора
}MENU;

MENU *CreateSMenu(WSHDR **ws1, WSHDR **ws2, IMGHDR **icons, int icon_flag, void **procs, int y, int d_items, int total);
void DrawSMenu(MENU *menu);
void DestroySMenu(MENU *menu);
void ActionSMenu(MENU *menu);
void OnKeySMenu(MENU *menu, unsigned int way);

#endif
