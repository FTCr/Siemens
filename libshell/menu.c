#include <swilib.h>
#include "../libsiemens/graphics.h"
#include "graphics.h"
#include "menu.h"
#include "config_col.h"
#include "config_coord.h"

MENU *CreateSMenu(char **strings1, char **strings2, int encoding, IMGHDR **icons, int icon_flag, void **procs, int y, int d_items, int total)
{	
	MENU *menu = malloc(sizeof(MENU));
	menu->ws         = AllocWS(128);
	menu->items      = NULL;
	menu->first_item = 1;
	menu->cur_id     = 0;
	menu->total      = total;
	menu->y          = y;
	menu->max        = (total < d_items) ? total : d_items;
	menu->data       = menu->max - 1;
	menu->enc        = encoding;
	
	unsigned int len;
	for(int i = 0; i < menu->total; i++)
	{
		menu->items = realloc(menu->items, sizeof(MENU_ITEM*) * (i + 2));
		menu->items[i] = malloc(sizeof(MENU_ITEM));
		zeromem(menu->items[i], sizeof(MENU_ITEM));
		menu->items[i + 1] = NULL;
		if (strings1)
		{
			menu->items[i]->string1 = strings1[i];
		}
		if (strings2)
		{
			menu->items[i]->string2 = strings2[i];
		}
		if (icons)
		{
			if (icon_flag == MENU_MANY_ICONS)
				menu->items[i]->icon = icons[i];
			else
				menu->items[i]->icon = icons[0];
		}
		if (procs)
		{
			menu->items[i]->proc = procs[i];
		}
	}
	return menu;
}


void DestroySMenu(MENU *menu)
{
	if (menu)
	{
		if (menu->items)
		{
			unsigned int i = 0;
			while(menu->items[i] != NULL)
			{
				mfree(menu->items[i++]);
			}
			FreeWS(menu->ws);
			mfree(menu->items);
		}
		mfree(menu);
	}
}


void OnKeySMenu(MENU *menu, unsigned int way)
{
	if (way == MENU_DOWN)
	{
		menu->cur_id++;
		if (menu->cur_id != menu->total - 1)
		{
			if (menu->cur_id == menu->data)
			{
				menu->first_item++;
				menu->data++;
			}
			else if(menu->cur_id == menu->total)
			{
				menu->cur_id = 0;
				menu->first_item = 1;
				menu->data = menu->max - 1;
			}
		}
		DrawSMenu(menu);
	}
	else if (way == MENU_UP)
	{
		menu->cur_id--;
		if (menu->first_item != 1)
		{
			if (menu->cur_id == menu->data - (menu->max - 1))
			{
				menu->first_item--;
				menu->data--;
			}
		}
		else if(menu->cur_id == -1)
		{
			menu->cur_id     = menu->total - 1;
			menu->first_item = menu->total - menu->max + 1;
			menu->data       = menu->total - 1;
		}
		DrawSMenu(menu);
	}
}

void ActionSMenu(MENU *menu)
{
	if (menu->items[menu->cur_id]->proc)
		((void(*)())menu->items[menu->cur_id]->proc)();
}

void DrawSMenu(MENU *menu)
{
	void ws_enc_work(char *str)
	{
		if (menu->enc == MENU_ENC_CP1251)
			wsprintf(menu->ws, "%t", str);
		else
			str_2ws(menu->ws, str, strlen(str));
	}
	
	if (menu->items)
	{
		const int Height_item = img[imgCursor]->h + 1;
		
		const int Start_y  = menu->y;
		const int Cur_x_off = (ScreenW() - img[imgCursor]->w) / 2;
		
		unsigned int font = FONT_MEDIUM;
		unsigned int x = Cur_x_off;
		unsigned int y = 0;
		DrawSeparateBG(0, Start_y, ScreenW(), Start_y + Height_item * menu->max);
		
		
		unsigned int n = menu->first_item - 1;
		y = Start_y + Height_item * (menu->cur_id - n) + 1;
		DrawIMGHDR(img[imgCursor], Cur_x_off, y, 0, 0, 0, 0);
		
		//все остальное
		int i = 0;
		while(i != menu->max)
		{
			y = Start_y + Height_item * i;
			DrawLine(Cur_x_off, y, ScreenW() - Cur_x_off, y, 1, cfg_col_menu_line);
			x = Cur_x_off + 5;
			if(menu->items[n]->icon)
			{
				y = Start_y + Height_item * i + (img[imgCursor]->h - menu->items[n]->icon->h) / 2 + 1;
				DrawIMGHDR(menu->items[n]->icon, x, y, 0, 0, 0, 0);
				x = Cur_x_off + 5 + menu->items[n]->icon->w + 5;
			}
			//дополнительная строка
			
			if (menu->items[n]->string2)
			{
				y = Start_y + Height_item * i + img[imgCursor]->h - GetFontYSIZE(FONT_SMALL);
				
				ws_enc_work(menu->items[n]->string2);
				DrawStringWS(menu->ws, x, y, x + img[imgCursor]->w, y + GetFontYSIZE(FONT_SMALL), FONT_SMALL, TEXT_ALIGNLEFT,
						cfg_col_menu_main_add, GetPaletteAdrByColorIndex(23));
				font = FONT_SMALL_BOLD;
				y = Start_y + Height_item * i;
			}
			else
			{
				font = FONT_MEDIUM;
				y = Start_y + Height_item * i + (img[imgCursor]->h - GetFontYSIZE(font)) / 2;
			}
			//основная строка
			ws_enc_work(menu->items[n]->string1);
			DrawStringWS(menu->ws, x, y, x + img[imgCursor]->w, y + GetFontYSIZE(font), font, TEXT_ALIGNLEFT, cfg_col_menu_main,
				GetPaletteAdrByColorIndex(23));
			i++;
			n++;
		}
		//линия в конце
		y = Start_y + Height_item * i;
		DrawLine(Cur_x_off, y, ScreenW() - Cur_x_off, y, 1, cfg_col_menu_line);
	}
}
