#include <swilib.h>
#include "../../../libsiemens/lang.h"
#include "../../../libsiemens/other.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/skin_utils.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/config_coord.h"
#include "../../../libshell/other.h"
#include "conf_loader.h"
#include "config_data.h"

char **lgp;

WSHDR *ws;
WSHDR *ws_main;

unsigned int is_ussd;
unsigned int *desk_id_ptr;

enum
{
	lgpHeader,
	lgpSoftkeyLeft1,
	lgpSoftkeyLeft2,
	lgpSoftkeyRight,
	lgpNoUSSD
};

void OnRedraw(void)
{
	unsigned int font_main = GetFont(fontMain1);
	DrawText(ws, lgp[lgpHeader], GetFont(fontHeader1), TEXT_ALIGNMIDDLE, GetColor(colorHeader), TEXT_TYPE_HEADER);
	if (is_ussd)
	{
		DrawString(ws_main, 0, cfg_coord_h_mid - GetFontYSIZE(font_main)/2, ScreenW(), ScreenH(), font_main, TEXT_ALIGNMIDDLE,
					GetColor(colorMain), GetPaletteAdrByColorIndex(23));
		DrawText(ws, lgp[lgpSoftkeyLeft2], GetFont(fontSoftkeys), TEXT_ALIGNLEFT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	}
	else
	{
		wsprintf(ws_main, "%t", lgp[lgpNoUSSD]);
		DrawString(ws_main, 0, cfg_coord_h_mid - GetFontYSIZE(font_main)/2, ScreenW(), ScreenH(), font_main, TEXT_ALIGNMIDDLE,
					GetColor(colorMain), GetPaletteAdrByColorIndex(23));
		DrawText(ws, lgp[lgpSoftkeyLeft1], GetFont(fontSoftkeys), TEXT_ALIGNLEFT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
	}
	DrawText(ws, lgp[lgpSoftkeyRight], GetFont(fontSoftkeys), TEXT_ALIGNRIGHT, GetColor(colorSoftkeys), TEXT_TYPE_SOFTKEY);
}

void OnKey(int key, int type)
{
	if (type == KEY_DOWN)
	{
		switch (key)
		{
			case LEFT_SOFT:
				if (is_ussd)
				{
					is_ussd = 0;
					DirectRedrawGUI_ID(shell_gui_id);
				}
				else
				{
					ExecFunc("MSG_CREATE_SMS");
				}
			break;
			case RIGHT_SOFT:
				ExecFile(cfg_mss_path);
			break;
		}
	}
}

void Destroy(void)
{
	FreeLang(&lgp);

	FreeWS(ws);
	FreeWS(ws_main);
}

void ProcessUSSD(CSM_RAM* data, GBS_USSD_MSG *msg)
{
	int len = msg->pkt_length;
	if (len>240) len = 240;
	GSMTXT_Decode(ws_main, msg->pkt, len, msg->encoding_type, (void*(*)(int))malloc_adr(), (void(*)(void))mfree_adr());
	FreeGSMTXTpkt(msg->pkt);
}

void OnMessage(CSM_RAM *data, GBS_MSG *msg)
{
	if(msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp(successed_config_filename, (char *)msg->data0) == 0)
		{
			InitConfig(successed_config_filename);
			*desk_id_ptr         = cfg_desk_id;
		}
	}
	if (msg->msg == MSG_USSD_RX || msg->msg == MSG_AUTOUSSD_RX)
	{
		is_ussd = 1;
		ProcessUSSD(data, (GBS_USSD_MSG *)msg);
		DirectRedrawGUI_ID(shell_gui_id);
	}
}

void main(PLUGIN_S4T *plg)
{
	char path[128];
	sprintf(path, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(path);

	plg->OnRedraw  = (void*)OnRedraw;
	plg->OnKey     = (void(*)(unsigned int, unsigned int))OnKey;
	plg->Destroy   = (void*)Destroy;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;

	plg->desk_id     = cfg_desk_id;
	desk_id_ptr = &plg->desk_id;

	sprintf(path, "%s%s", lang_dir, "mestab.txt");
	InitLang(path, &lgp);

	ws      = AllocWS(64);
	ws_main = AllocWS(256);
}
