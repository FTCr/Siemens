#include <swilib.h>
#include "../libsiemens/graphics.h"
#include "main.h"
#include "eeprom.h"
#include "71.h"
#include "218.h"

#define MAIN_MENU_ITEMS 0x03

void BackupQuest(int flag)
{
	if (!flag)
	{
		TTime time;
		TDate date;
		GetDateTime(&date, &time);
		
		//create dir
		unsigned int err;
		char dir_path[128];
		_mkdir("0:\\Misc\\EEPROM\\", &err);
		sprintf(dir_path, "%s%02d%02d%02d_%02d%02d%02d\\", "0:\\Misc\\EEPROM\\", date.year % 2000, date.month, date.day, time.hour, time.min, time.sec);
		_mkdir(dir_path, &err);
		
		
		int fp;
		char path[256];
		
		void *addr = NULL;
		unsigned int size;
		unsigned char ver;
		
		unsigned int i = 0;
		
		void SaveBlock()
		{
			sprintf(path, "%s%04d.bin", dir_path, i);
			fp = _open(path, A_WriteOnly + A_Create + A_BIN, P_WRITE, &err);
			_write(fp, addr, size, &err);
			_close(fp, &err);
		}
		LockSched();
		while (i <= GetEELiteMaxID())
		{
			if (GetEELiteBlockSizeVersion(i, &size, &ver) == 0)
			{
				addr = GetEELiteBlockAddress(i);
				SaveBlock();
			}
			i++;
		}
		while (i <= GetEEFullMaxID())
		{
			if (EEFullGetBlockInfo(i, &size, &ver) == 0)
			{
				addr = malloc(size);
				EEFullReadBlock(i, addr, 0, size, 0, 0);
				SaveBlock();
				mfree(addr);
			}
			i++;
		}
		UnlockSched();
	}
}

void Backup(GUI *data)
{
	MsgBoxYesNo(1, (int)lgp[lgpQuest], BackupQuest);
}

void Editor71(GUI *data)
{
	Create71Editor();
}

void Editor218(GUI *data)
{
	Create218Editor();
}

/*void ClearExitsQuest(int flag)
{
	if (!flag)
	{
		int size;
		char ver;
		
		unsigned char _5261[28] = "\xCC\xFF\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x79\x82\x01\x00\x00\x00";
		EEFullWriteBlock(5261, _5261, 0, 28, 0, 0);
		
		void clear(int block)
		{
			EEFullGetBlockInfo(block, &size, &ver);
			char *buffer = malloc(size);
			EEFullReadBlock(block, buffer, 0, size, 0, 0);
			for (int i = 0; i < size; buffer[i++] = 0xFF);
			EEFullWriteBlock(block, buffer, 0, size, 0, 0);
			mfree(buffer);
		}
		clear(5335);
		clear(5336);
	}
}*/

/*void ClearExits(GUI *data)
{
	MsgBoxYesNo(1, (int)lgp[lgpQuest], ClearExitsQuest);
}*/

static HEADER_DESC header = {0, 0, 0, 0, NULL, (int)"EEPMan", LGP_NULL};

static int softkeys[] = {0, 1, 2};

static SOFTKEY_DESC sk[] =
{
	{0x0018, 0x0000, (int)"Select"},
	{0x0001, 0x0000, (int)"Exit"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB skt =
{
	sk, 0
};

MENUITEM_DESC items[MAIN_MENU_ITEMS]=
{
	{NULL, 0, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{NULL, 0, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{NULL, 0, LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2}
};

const MENUPROCS_DESC procs[MAIN_MENU_ITEMS]=
{
	Backup,
	Editor71,
	Editor218,
};

static MENU_DESC desc =
{
	8, NULL, NULL, NULL,
	softkeys,
	&skt,
	0x10,
	NULL,
	items,   //Items
	procs,   //Procs
	MAIN_MENU_ITEMS   //n
};

int CreateMainMenu(void)
{
	sk[0].lgp_id = (int)lgp[lgpSelect];
	sk[1].lgp_id = (int)lgp[lgpExit];
	for (int i = 0; i < MAIN_MENU_ITEMS; i++)
		items[i].lgp_id_small = (int)lgp[lgpSaveAllBlocks + i];
	
	patch_header(&header);
	return CreateMenu(0, 0, &desc, &header, 0, MAIN_MENU_ITEMS, NULL, NULL);
}
