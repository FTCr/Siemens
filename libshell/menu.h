#ifndef _LIBSHELL_MENU_H_
	#define _LIBSHELL_MENU_H_

#define MENU_UP   0x00
#define MENU_DOWN 0x01

#define MENU_ONE_ICON  0x00
#define MENU_MANY_ICON 0x01

typedef struct
{
	char *text1;
	char *text2;
	IMGHDR *icon;
	void *proc;
}MENU_ITEM;

typedef struct
{
	MENU_ITEM **items;
	WSHDR *ws;
	int first_item; //первый пункт для отрисовки
	int max;        //количество возможных пункто для отрисовки за один раз
	int data;       //вспомогательная переменная для перемещения курсора
	int cur_id;     //позиция курсора
	int total;      //общее число пунктов
}MENU;

MENU *CreateSMenu(WSHDR *ws, char **text1, char **text2, IMGHDR **icons, const unsigned int icon_flag, void **procs,
					const unsigned int total);
void DrawSMenu(MENU *menu);
void DestroySMenu(MENU *menu);
void ActionSMenu(MENU *menu);
void OnKeySMenu(MENU *menu, unsigned int way);

#endif
