#include <swilib.h>
#include "../libsiemens/strings.h"
#include "../libsiemens/lang.h"
#include "main.h"
#include "eeprom.h"
#include "main_menu.h"


typedef struct
{
	CSM_RAM csm;
	unsigned int gui_id;
} MAIN_CSM;

const int minus11 = -11;

char dir_path[128];
char global_fname_path[256];
unsigned int block;
char **lgp;

void maincsm_oncreate(CSM_RAM *data)
{
	MAIN_CSM *csm = (MAIN_CSM*)data;
	csm->gui_id = CreateMainMenu();
}

void maincsm_onclose(CSM_RAM *data)
{
	FreeLang(&lgp);
}


static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
	MAIN_CSM *csm = (MAIN_CSM*)data;
	if (msg->msg == MSG_GUI_DESTROYED)
	{
		if ((unsigned int)msg->data0 == csm->gui_id)
		{
			csm->csm.state =- 3;
		}
	}
	return 1;
}

unsigned short maincsm_name_body[140];

const struct
{
	CSM_DESC maincsm;
	WSHDR maincsm_name;
}MAINCSM =
{
	{
		maincsm_onmessage,
		maincsm_oncreate,
	#ifdef NEWSGOLD
		0,
		0,
		0,
		0,
	#endif
	maincsm_onclose,
	sizeof(MAIN_CSM),
	1,
	&minus11
	},
	{
		maincsm_name_body,
		NAMECSM_MAGIC1,
		NAMECSM_MAGIC2,
		0x0,
		139,
		0x0
	}
};


void UpdateCSMname(void)
{
	wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "EEPMan");
}

void WriteQuest(int flag)
{
	if (!flag)
	{
		FSTATS fs;
		unsigned int err;
		if (GetFileStats(global_fname_path, &fs, &err) != -1)
		{
			char *buffer = malloc(fs.size + 1);
			int fp = _open(global_fname_path, A_ReadOnly + A_BIN, P_READ, &err);
			_read(fp, buffer, fs.size, &err);
			_close(fp, &err);
			
			char ver;
			unsigned int size;
			if (block <= GetEELiteMaxID())
			{
				if (GetEELiteBlockSizeVersion(block, &size, &ver) == 0)
				{
					ShowMSG(1, (int)"Write");
					EELiteWriteBlock(block, buffer, fs.size, ver);
				}
			}
			else if (block <= GetEEFullMaxID())
			{
				if (EEFullGetBlockInfo(block, &size, &ver) == 0)
				{
					EEFullWriteBlock(block, buffer, 0, fs.size, 0, 0);
				}
			}
			mfree(buffer);
		}
	}
	kill_elf();
}

int main(const char *bin_path, const char *fname_path)
{
	GetDirByPath(dir_path, bin_path);
	char str[256];
	strcpy(str, dir_path);
	strcat(str, "\\");
	strcat(str, "lang.txt");
	if (InitLang(str, &lgp) != -1)
	{
		if (fname_path)
		{
			GetExtByPath(str, fname_path);
			if (strcmp_nocase(str, "bin") == 0)
			{
				char name[8];
				if (GetFileNameWithoutExtByPath(name, fname_path) != -1)
				{
					sscanf(name, "%d", &block);
					strcpy(global_fname_path, fname_path);
					MsgBoxYesNo(1, (int)lgp[lgpQuest], WriteQuest);
				}
				else
					kill_elf();
			}
			else
				kill_elf();
		}
		else
		{
			char dummy[sizeof(MAIN_CSM)];
			UpdateCSMname();
			LockSched();
			CreateCSM(&MAINCSM.maincsm, dummy, 0);
			UnlockSched();
		}
	}
	else
	{
		MsgBoxError(1, (int)"lang.txt not found!");
		kill_elf();
	}
	return 0;
}
