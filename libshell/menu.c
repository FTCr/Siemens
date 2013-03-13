#include <swilib.h>
#include "../libsiemens/graphics.h"
#include "graphics.h"
#include "menu.h"
#include "config_col.h"
#include "config_coord.h"

MENU *CreateSMenu(WSHDR *ws, char **text1, char **text2, IMGHDR **icons, const unsigned int icon_flag, void **procs,
					const unsigned int total)
{	
	MENU *menu = malloc(sizeof(MENU));
	menu->items      = NULL;
	menu->ws         = ws;
	menu->first_item = 1;
	menu->max        = (total < cfg_coord_max_menu_items) ? total : cfg_coord_max_menu_items;
	menu->data       = menu->max - 1;
	menu->cur_id     = 0;
	menu->total      = total;
	
	unsigned int len;
	for(unsigned int i = 0; i < total; i++)
	{
		menu->items = realloc(menu->items, sizeof(MENU_ITEM*) * (i + 2));
		menu->items[i] = malloc(sizeof(MENU_ITEM));
		zeromem(menu->items[i], sizeof(MENU_ITEM));
		menu->items[i + 1] = NULL;
		if (text1)
		{
			menu->items[i]->text1 = text1[i];
		}
		if (text2)
		{
			menu->items[i]->text2 = text2[i];
		}
		if (icons)
		{
			if (icon_flag == MENU_MANY_ICON)
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
	((void(*)())menu->items[menu->cur_id]->proc)();
}

void DrawSMenu(MENU *menu)
{
	if (menu->items)
	{
		const int Height_item = img[imgCursor]->h + 1;
		const int Start_y  = ICONBAR_H + img[imgHeader]->h + 1;
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
			if (menu->items[n]->text2)
			{
				y = Start_y + Height_item * i + img[imgCursor]->h - GetFontYSIZE(FONT_SMALL);
				wsprintf(menu->ws, "%t", menu->items[n]->text2);
				DrawString(menu->ws, x, y, x + img[imgCursor]->w, y + GetFontYSIZE(FONT_SMALL), FONT_SMALL, TEXT_ALIGNLEFT,
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
			wsprintf(menu->ws, "%t", menu->items[n]->text1);
			DrawString(menu->ws, x, y, x + img[imgCursor]->w, y + GetFontYSIZE(font), font, TEXT_ALIGNLEFT,
						cfg_col_menu_main, GetPaletteAdrByColorIndex(23));
			i++;
			n++;
		}
		//линия в конце
		y = Start_y + Height_item * i;
		DrawLine(Cur_x_off, y, ScreenW() - Cur_x_off, y, 1, cfg_col_menu_line);
	}
}
