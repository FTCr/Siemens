#include <swilib.h>
#include "../../../libshell/plugins.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/skin_utils.h"
#include "../../../libshell/other.h"
#include "../../../libsiemens/other.h"
#include "../../../libsiemens/lang.h"
#include "conf_loader.h"
#include "config_data.h"

char *RamMissedCalls(void)
__swi(0x80CC);

char *RamMissedMes(void)
__swi(0x80D4);

enum
{
	lgpSoftkeyLeft,
};

char **lgp;

WSHDR *ws;

unsigned int *desk_id_ptr;

unsigned int IsMissed(void)
{
	return ((*RamMissedCalls()) || (*RamMissedMes())) ? 1 : 0;
}

void OnRedraw(void)
{
	if (IsMissed())
	{
		char str[128];
		sprintf(str, "%s (%d)", lgp[lgpSoftkeyLeft], (*RamMissedCalls()) + (*RamMissedMes()));
		DrawText(ws, str, GetFont(fontSoftkeys), (cfg_softkey) ? TEXT_ALIGNRIGHT : TEXT_ALIGNLEFT, GetColor(colorSoftkeys),
			TEXT_TYPE_SOFTKEY);
	}
	else
		DrawText(ws, lgp[lgpSoftkeyLeft], GetFont(fontSoftkeys), (cfg_softkey) ? TEXT_ALIGNRIGHT : TEXT_ALIGNLEFT, GetColor(colorSoftkeys),
			TEXT_TYPE_SOFTKEY);
}

void OnKey(unsigned int key, unsigned int type)
{
	void Inbox(void)
	{
		char *ptr = 0;
		ptr = RamMissedCalls();
		*ptr = 0;
		ptr = RamMissedMes();
		*ptr = 0;
		ExecShortcut("DEVICE_INBOX");
	}
	
	if (type == KEY_DOWN)
	{
		switch (key)
		{
			case LEFT_SOFT:
				if(cfg_softkey == 0) Inbox();
			break;
			case RIGHT_SOFT:
				if (cfg_softkey == 1) Inbox();
			break;
		}
	}
}

void Destroy(void)
{
	if (lgp)
		FreeLang(&lgp);
	if (ws)
		FreeWS(ws);
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
}

int main(PLUGIN_S4T *plg)
{
	char path[128];
	sprintf(path, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(path);

	plg->OnRedraw  = (void*)OnRedraw;
	plg->OnKey     = (void(*)(unsigned int, unsigned int))OnKey;
	plg->Destroy   = (void*)Destroy;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;

	sprintf(path, "%s%s", lang_dir, "sinbox.txt");
	if (InitLang(path, &lgp) == -1) return -1;

	plg->desk_id = cfg_desk_id;
	desk_id_ptr  = &plg->desk_id;
	
	ws = AllocWS(64);
	
	return 0;
}
