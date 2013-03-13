#include <swilib.h>
#include "../../../libshell/plugins.h"
#include "../../../libshell/skin_utils.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/other.h"
#include "conf_loader.h"
#include "config_data.h"

int GetProvider(WSHDR *ws, int zero)
__swi(0x41);

WSHDR *ws;
unsigned int *desk_id_ptr;

void OnRedraw(void)
{
	TDate dt;
	char str[16];
	unsigned int font = GetFont(fontHeader2);

	GetDateTime(&dt, NULL);
	sprintf(str, "%02d.%02d.%02d", dt.year % 2000, dt.month, dt.day);
	DrawText(ws, str, font, TEXT_ALIGNLEFT, GetColor(colorHeader), TEXT_TYPE_HEADER);

	GetProvider(ws, 0);
	ws_2str(ws, str, ws->wsbody[0]);
	DrawText(ws, str, font, TEXT_ALIGNRIGHT, GetColor(colorHeader), TEXT_TYPE_HEADER);
}

void Destroy(void)
{
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

void main(PLUGIN_S4T *plg)
{
	//чтение конфига
	char bcfg[128];
	sprintf(bcfg, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(bcfg);

	plg->OnRedraw  = (void*)OnRedraw;
	plg->Destroy   = (void*)Destroy;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;

	plg->desk_id = cfg_desk_id;
	desk_id_ptr  = &plg->desk_id;

	ws = AllocWS(32);
}
