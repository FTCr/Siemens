#include <swilib.h>
#include "../libsiemens/graphics.h"
#include "main.h"
#include "eeprom.h"

static HEADER_DESC header = {0, 0, 0, 0, NULL, (int)"Edit BT address", LGP_NULL};

extern int softkeys[];

static SOFTKEY_DESC sk[]=
{
	{0x0018, 0x0000, (int)"Write"},
	{0x0001, 0x0000, (int)"Back"},
	{0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB skt =
{
	sk, 0
};

static int OnKey(GUI *gui, GUI_MSG *msg)
{
	if (msg->keys == 0x18)
	{
		EDITCONTROL ec;
		ExtractEditControl(gui, 1, &ec);
		
		int len = ec.pWS->wsbody[0];
		if (len != 12) return 0;
		
		char bt_addr[12];
		ws_2str(ec.pWS, bt_addr, len);
		for (int i = 0; i < 12; i++)
		{
			if ((bt_addr[i] >= '0' && bt_addr[i] <= '9') || (bt_addr[i] >= 'A' && bt_addr[i] <= 'F') || (bt_addr[i] >= 'a' && bt_addr[i] <= 'f'))
				continue;
			else
			{
				return 0;
			}
		}
		
		/*
		 * хуета какая-то
		 */
		char hex[3];
		hex[2] = '\0';
		int num[6];
		int matches = 0;
		for (int i = 0; i < 6; i++)
		{
			strncpy(hex, bt_addr + i * 2, 2);
			sscanf(hex, "%x", &num[i]);
			if (num[0] == num[i]) matches++;
		}
		//при полном соответствии всех цифр адреса, пищит при включении
		if (matches == 6)
			return 0;
		
		char buffer[6];
		sprintf(buffer, "%c%c%c%c%c%c", num[5], num[4], num[3], num[2], num[1], num[0]);
		int size;
		char ver;
		GetEELiteBlockSizeVersion(218, &size, &ver);
		EELiteWriteBlock(218, buffer, 6, ver);
		return 1;
		
	}
	return 0;
}

static void GHook(GUI *gui, int cmd)
{
	if (cmd == TI_CMD_REDRAW)
	{
		static SOFTKEY_DESC sk_l = {0x0018, 0x0000, LGP_NULL};
		sk_l.lgp_id = (int)lgp[lgpWrite];
		SetSoftKey(gui, &sk_l, SET_SOFT_KEY_N);
	}
}


static INPUTDIA_DESC desc =
{
	1, OnKey, GHook, NULL,
	0,
	&skt,
	{0, 0, 0, 0},
	FONT_MEDIUM,
	100,
	101,
	0,
	0,
	0x40000000
};

void Create218Editor(void)
{	
	void *ma = malloc_adr();
	void *eq = AllocEQueue(ma, mfree_adr());
	EDITCONTROL ec;
	PrepareEditControl(&ec);
	
	char *buffer = malloc(6);
	EELiteReadBlock(218, buffer, 0, 6);
	
	int num[6];
	for (int i = 0; i < 6; i++)
		num[5 - i] = buffer[i];
	
	WSHDR *ws = AllocWS(13);
	wsprintf(ws, "%02x%02x%02x%02x%02x%02x", num[0], num[1], num[2], num[3], num[4], num[5]);
	ConstructEditControl(&ec, ECT_NORMAL_TEXT, ECF_DISABLE_T9, ws, 12);
	AddEditControlToEditQend(eq, &ec, ma);
	FreeWS(ws);
	
	patch_header(&header);
	patch_input(&desc);
	
	
	sk[0].lgp_id  = (int)lgp[lgpWrite];
	header.lgp_id = (int)lgp[lgpEditBT];
	
	CreateInputTextDialog(&desc, &header, eq, 1, NULL);
}