#include <swilib.h>
#include "../libsiemens/graphics.h"
#include "main.h"
#include "eeprom.h"

#define GET_FLAG()    ((_71->data[cur_item / 8 + _71->offset]) >> (7 - (cur_item % 8)) & 1)
#define TOGGLE_FLAG() _71->data[cur_item / 8 + _71->offset] ^= 1 << (7 - (cur_item % 8))

typedef struct
{
	char *data;
	int size;
	char ver;
	
	//смещение до настроек
	int offset;
}_71;

typedef struct
{
	char *str;
	int id;
} _71_LGP;

_71_LGP *_71_lgp;
int _71_lgp_total;

static HEADER_DESC header = {0, 0, 0, 0, NULL, 0, LGP_NULL};

static int softkeys[] = {0, 1, 2};

static SOFTKEY_DESC sk[] =
{
	{0x0018, 0x0000, (int)"Write"},
	{0x0001, 0x0000, (int)"Back"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB skt =
{
	sk, 0
};

const int icons[] = {ICON_CBOX_UNSEL, ICON_CBOX_SEL};

int InitLGP(void)
{
	FSTATS fs;
	unsigned int err;
	char path[256];
	strcpy(path, dir_path);
	strcat(path, "\\");
	strcat(path, "71.txt");
	
	if (GetFileStats(path, &fs, &err) == -1) return 0;

	int fp = _open(path, A_ReadOnly + A_BIN, P_READ, &err);
	if (fp == -1) return 0;

	char *buffer = malloc(fs.size + 1);
	buffer[fs.size] = '\0';
	_read(fp, buffer, fs.size, &err);
	
	char tmp[128];
	char tmp_id[4];
	unsigned int tmp_id_len = 0;
	unsigned int wait_num = 1;
	
	unsigned int pos = 0, len = 0;
	while (pos <= fs.size)
	{
		if (buffer[pos] == '\n' || buffer[pos] == '\r' || buffer[pos] == '\0')
		{
			if (len)
			{
				_71_lgp = realloc(_71_lgp, sizeof(_71_LGP) * (_71_lgp_total + 1));
				_71_lgp[_71_lgp_total].str = malloc(len + 1);
				
				tmp[len] = '\0';
				strcpy(_71_lgp[_71_lgp_total].str, tmp);
				sscanf(tmp_id, "%d", &(_71_lgp[_71_lgp_total].id));
				len = 0;
				tmp_id_len = 0;
				wait_num = 1;
				
				_71_lgp_total++;
			}
		}
		else
		{
			if (wait_num)
			{
				if (buffer[pos] != ',')
					tmp_id[tmp_id_len++] = buffer[pos];
				else
				{
					tmp_id[tmp_id_len] = '\0';
					wait_num = 0;
				}
			}
			else
			{
				tmp[len++] = buffer[pos];
			}
		}
		pos++;
	}
	mfree(buffer);
	_close(fp, &err);
	return 1;
}

static int OnKey(void *data, GUI_MSG *msg)
{
	if (msg->keys == 0x3D)
	{
		int cur_item = GetCurMenuItem(data);
		_71 *_71 = MenuGetUserPointer(data);
		TOGGLE_FLAG();
		RefreshGUI();
	}
	else if (msg->keys == 0x18)
	{
		_71 *_71 = MenuGetUserPointer(data);
		EELiteWriteBlock(71, _71->data, _71->size, _71->ver);
		return 1;
	}
	return 0;
}

static void GHook(void *data, int cmd)
{
	if (cmd == TI_CMD_DESTROY)
	{
		_71 *_71 = MenuGetUserPointer(data);
		mfree(_71->data);
		mfree(_71);
		
		for (int i = 0; i < _71_lgp_total; mfree(_71_lgp[i++].str));
		
		mfree(_71_lgp);
		_71_lgp = NULL;
		_71_lgp_total = 0;
	}
}

static void Handler(void *data, int cur_item, void *user_pointer)
{
	void *item = AllocMenuItem(data);
	WSHDR *ws = AllocMenuWS(data, 128);
	
	
	for (int i = 0; i < _71_lgp_total; i++)
	{
		if (cur_item == _71_lgp[i].id)
		{
			wsprintf(ws, "%t", _71_lgp[i].str);
			break;
		}
		else
			wsprintf(ws, "%t", "Unknown");
	}
	
	
	_71 *_71 = MenuGetUserPointer(data);
	int flag = GET_FLAG();
	SetMenuItemIconArray(data, item, icons);
	SetMenuItemIcon(data, cur_item, flag);
	SetMenuItemText(data, item, ws, cur_item);
}

static MENU_DESC desc =
{
	8, OnKey, GHook, NULL,
	softkeys,
	&skt,
	0x11,
	Handler,
	NULL,   //Items
	NULL,   //Procs
	0   //n
};

void Create71Editor(void)
{
	if (InitLGP())
	{
		char ver;
		unsigned int size;
		if (GetEELiteBlockSizeVersion(71, &size, &ver) == 0)
		{
			_71 *_71 = malloc(sizeof(_71));
			_71->data = malloc(size);
			_71->size = size;
			_71->ver = ver;
			
			if (EELiteReadBlock(71, _71->data, 0, size + 1) != -1)
			{
				_71->offset = _71->data[0] * 3 + 1;
				int items = _71->data[_71->offset];
				_71->offset += 3;
				
				sk[0].lgp_id  = (int)lgp[lgpWrite];
				sk[1].lgp_id  = (int)lgp[lgpBack];
				header.lgp_id = (int)lgp[lgpEdit71];
				patch_header(&header);
				CreateMenu(0, 0, &desc, &header, 0, items, _71, NULL);
			}
		}
	}
	else
		MsgBoxError(1, (int)"Error");
}