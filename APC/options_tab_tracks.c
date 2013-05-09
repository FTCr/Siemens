#include <swilib.h>
#include <libapd.h>
#include "../libsiemens/graphics.h"
#include "../libsiemens/find_files.h"
#include "main.h"
#include "playback.h"
#include "tab_tracks.h"
#include "playlist_save.h"
#include "help.h"
#include "about.h"

#define OPTIONS_ITEMS_N   0x0A
#define OPTIONS_ITEMS_N_2 0x02

const int icon_empty = ICON_EMPTY;

static HEADER_DESC header = {0, 0, 0, 0, NULL, (int)"Options", LGP_NULL};

static MENUITEM_DESC items[OPTIONS_ITEMS_N] =
{
	{(int*)&icon_empty, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{(int*)&icon_empty, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{(int*)&icon_empty, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{(int*)&icon_empty, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{(int*)&icon_empty, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{(int*)&icon_empty, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{(int*)&icon_empty, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{(int*)&icon_empty, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{(int*)&icon_empty, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{(int*)&icon_empty, LGP_NULL, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
};

static void Toggle(GUI *data)
{
	if (APlayer_GetPlayStatus() != APLAYER_PLAY)
		APlayer_Play();
	else
		APlayer_Stop();
	GeneralFuncF1(2);
}

static void PlayMode(GUI *data)
{
	CreatePlaybackMenu();
	GeneralFuncF1(2);
}

void CutItem(GUI *gui)
{
	unsigned int i = GetCurMenuItem(gui);
	APlayer_CutFile(i);
	
	unsigned int total = APlayer_GetTotalTracks();
	if (total)
	{
		if (i == total)
		{
			SetCursorToMenuItem(gui, i - 1);
		}
	}
	Menu_SetItemCountDyn(gui, APlayer_GetTotalTracks());
}

static void Cut(GUI *data)
{
	CutItem(tab_tracks_gui);
	GeneralFuncF1(2);
}

static void Paste(GUI *data)
{
	unsigned int i = GetCurMenuItem(tab_tracks_gui);
	APlayer_PasteFile(i);
	unsigned int total = APlayer_GetTotalTracks();
	
	Menu_SetItemCountDyn(tab_tracks_gui, total);
	
	GeneralFuncF1(2);
}

static void Clear(GUI *data)
{
	APlayer_ClearPlayList();
	Menu_SetItemCountDyn(tab_tracks_gui, APlayer_GetTotalTracks());
	GeneralFuncF1(2);
}

static void SavePlaylist(GUI *data)
{
	CreatePlaylistSaveGUI();	
	GeneralFuncF1(2);
}

/*static void TrackInfo(GUI *data)
{
	unsigned int i = GetCurMenuItem(m_gui);
	DIR_ENTRY_LIST *ptr = APlayer_GetPtr(i);
	if (!CreateTrackInfoGUI(ptr->data))
		MsgBoxError(1, (int)"Empty tag!");
	GeneralFuncF1(2);
}*/

static void Send(GUI *data)
{
	unsigned int i = GetCurMenuItem(tab_tracks_gui);
	DIR_ENTRY_LIST *ptr = APlayer_GetPtr(i);
	
	WSHDR *fname, *dir;
	int len;
	
	len = strlen(ptr->fname);
	fname = AllocWS(len);
	str_2ws(fname, ptr->fname, len);
	
	len = strlen(ptr->dir);
	dir = AllocWS(len);
	str_2ws(dir, ptr->dir, len);
	
	MediaSendCSM_Open(fname, dir);
	
	FreeWS(fname);
	FreeWS(dir);
	
	GeneralFuncF1(2);
}

void Delete(int flag)
{
	if (!flag)
	{
		unsigned int i = GetCurMenuItem(tab_tracks_gui);
		if (APlayer_GetTrack() - 1 == i)
		{
			MsgBoxError(1, (int)"Error!");
		}
		else
		{
			DIR_ENTRY_LIST *ptr = APlayer_GetPtr(i);
			unsigned int err;
			_unlink(ptr->path, &err);
			APlayer_CutFile(i);
			Menu_SetItemCountDyn(tab_tracks_gui, APlayer_GetTotalTracks());
		}
	}
}

static void DeleteQuest(GUI *data)
{
	MsgBoxYesNo(1, (int)lgp[lgpDelQ], Delete);
	GeneralFuncF1(2);
}

void Help(GUI *data)
{
	CreateHelpGUI();
	GeneralFuncF1(2);
}

void About(GUI *data)
{
	CreateAboutGUI();
	GeneralFuncF1(2);
}

static const MENUPROCS_DESC procs[OPTIONS_ITEMS_N] =
{
	Toggle,
	PlayMode,
	Cut,
	Paste,
	Clear,
	SavePlaylist,
	Send,
	DeleteQuest,
	Help,
	About
};

static const MENUPROCS_DESC procs2[OPTIONS_ITEMS_N_2] =
{
	Help,
	About
};

extern int softkeys[];

static SOFTKEY_DESC sk[] =
{
	{0x0018, 0x0000, (int)"Select"},
	{0x0001, 0x0000, (int)"Back"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB skt =
{
	sk, 0
};

static MENU_DESC desc=
{
	8, NULL, NULL, NULL,
	softkeys,
	&skt,
	0x11,
	NULL,
	//Handler,
	items,   //Items
	NULL,   //Procs
	0 //n
};

void CreateOptionsTabTracks(void)
{
	patch_header_small((HEADER_DESC*)(&header));
	
	header.lgp_id = (int)lgp[lgpOptions];
	
	sk[0].lgp_id = (int)lgp[lgpOptionsSelect];
	sk[1].lgp_id = (int)lgp[lgpOptionsBack];
	
	if (APlayer_GetTotalTracks())
	{
		if (APlayer_GetPlayStatus() == APLAYER_STOP || APlayer_GetPlayStatus() == APLAYER_PAUSE)
			items[0].lgp_id_small = (int)lgp[lgpOptionsPlay];
		else
			items[0].lgp_id_small = (int)lgp[lgpOptionsStop];
		for (int i = 0; i < OPTIONS_ITEMS_N - 1; i++)
		{
			items[i + 1].lgp_id_small = (int)lgp[lgpOptionsPlayMode + i];
		}	
		
		desc.n_items = OPTIONS_ITEMS_N;
		desc.procs = procs;
		CreateMenu(1, 0, &desc, &header, 0, OPTIONS_ITEMS_N, NULL, NULL);
	}
	else
	{
		for (int i = 0; i < OPTIONS_ITEMS_N_2; i++)
			items[i].lgp_id_small = (int)lgp[lgpOptionsHelp + i];
		desc.n_items = OPTIONS_ITEMS_N_2;
		desc.procs = procs2;
		CreateMenu(1, 0, &desc, &header, 0, OPTIONS_ITEMS_N_2, NULL, NULL);
	}
}
