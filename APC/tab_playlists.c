#include <swilib.h>
#include "libapd.h"
#include "../libsiemens/find_files.h"
#include "../libsiemens/strings.h"
#include "main.h"
#include "tab_tracks.h"

const int icon = ICON_M3U_SMALL;

static SOFTKEY_DESC SK_TabPlaylists[]=
{
	{0x0018, 0x0000, (int)"Options"},
	{0x0001, 0x0000, (int)"Exit"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static int OnKey_TabPlaylists(void *data, GUI_MSG *msg)
{
	const int Key   = msg->gbsmsg->submess;
	const int Style = msg->gbsmsg->msg;
	if (Style == KEY_DOWN)
	{
		DIR_ENTRY_LIST *p = NULL, *p2;
		switch (Key)
		{
			case LEFT_SOFT:
				p = MenuGetUserPointer(data);
				if (p)
				{
					APlayer_ClearPlayList();
					p2 = GetDEListPtr(p, GetCurMenuItem(data));
					APlayer_OpenPlayList(p2->path);
					Menu_SetItemCountDyn(tab_tracks_gui, APlayer_GetTotalTracks());
					RefreshMainGUI();
				}
			break;
			case ENTER_BUTTON:
				p = MenuGetUserPointer(data);
				if (p)
				{
					p2 = GetDEListPtr(p, GetCurMenuItem(data));
					APlayer_OpenPlayList(p2->path);
					Menu_SetItemCountDyn(tab_tracks_gui, APlayer_GetTotalTracks());
					RefreshMainGUI();
				}
			break;
		}
	}
	return 0;
}

static void GHook_TabPlaylists(void *data, int cmd)
{
	if (cmd == TI_CMD_CREATE)
	{
		void *p = MenuGetUserPointer(data);
		if (p)
		{
			SK_TabPlaylists[0].lgp_id  = (int)lgp[lgpReplace];
		}
		else
		{
			SK_TabPlaylists[0].lgp_id = (int)" ";
			SK_TabPlaylists[2].lgp_id = (int)" ";
		}
		
		SK_TabPlaylists[1].lgp_id  = (int)lgp[lgpExit];
	}
	if (cmd == TI_CMD_DESTROY)
	{
		DIR_ENTRY_LIST *top = MenuGetUserPointer(data);
		FreeDEList(top, NULL);
	}
}


static void Handler_TabPlaylists(void *data, int cur_item, void *user_pointer)
{
	void *item = AllocMLMenuItem(data);
	
	DIR_ENTRY_LIST *ptr = GetDEListPtr(user_pointer, cur_item);
	WSHDR *ws1;
	WSHDR *ws2;
	unsigned int len;
	if (ptr)
	{
		char fname[128];
		if (GetFileNameWithoutExtByFileName(fname, ptr->fname) != -1)
		{
			len = strlen(fname);
			ws1 = AllocMenuWS(data, len);
			str_2ws(ws1, fname, len);
		}
		else
		{
			len = 64;
			ws1 = AllocMenuWS(data, len);
			str_2ws(ws1, "Error GetFileNameWithoutExt", len);
		}
		len = strlen(ptr->path);
		ws2 = AllocMenuWS(data, len);
		str_2ws(ws2, ptr->path, len);
	}
	else
	{
		len = 32;
		ws1 = AllocMenuWS(data, len);
		ws2 = AllocMenuWS(data, len);
		str_2ws(ws1, "Error DIR_ENTRY_LIST", len);
		str_2ws(ws2, "Error DIR_ENTRY_LIST", len);
	}
	SetMenuItemIconArray(data, item, &icon);
	SetMLMenuItemText(data, item, ws1, ws2, cur_item);
}

extern int softkeys[];

static SOFTKEYSTAB skt =
{
	SK_TabPlaylists, 0
};

ML_MENU_DESC Desc_TabPlaylists =
{
	8, OnKey_TabPlaylists, GHook_TabPlaylists, NULL,
	softkeys,
	&skt,
	0x11,
	Handler_TabPlaylists,
	NULL,
	NULL,
	0,
	1
};