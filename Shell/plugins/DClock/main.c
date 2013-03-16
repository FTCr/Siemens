#include <swilib.h>
#include "../../../libsiemens/graphics.h"
#include "../../../libsiemens/other.h"
#include "../../../libshell/other.h"
#include "../../../libshell/graphics.h"
#include "../../../libshell/plugins.h"
#include "conf_loader.h"
#include "config_data.h"

unsigned int *desk_id_ptr;

IMGHDR *bg;
IMGHDR *digits[10];

GBSTMR tmr;
WSHDR *ws;


void Draw(void)
{
	TTime tm;
	GetDateTime(NULL, &tm);
	unsigned int x;
	unsigned int cleft, cright;
	//background
	DrawIMGHDR(bg, cfg_pos_x, cfg_pos_y, 0, 0, 0, 0);
	//hours
	cleft  = tm.hour / 10;
	cright = tm.hour % 10;
	x      = cfg_pos_x + cfg_h_offset_x;
	DrawIMGHDR(digits[cleft], x, cfg_pos_y + cfg_offset_y, 0, 0, 0, 0);
	x += digits[cleft]->w;
	DrawIMGHDR(digits[cright], x, cfg_pos_y + cfg_offset_y, 0, 0, 0, 0);

	//minutes
	x      = cfg_pos_x + cfg_m_offset_x;
	cleft  = tm.min / 10;
	cright = tm.min % 10;
	DrawIMGHDR(digits[cleft], x, cfg_pos_y + cfg_offset_y, 0, 0, 0, 0);
	x += digits[cleft]->w;
	DrawIMGHDR(digits[cright], x, cfg_pos_y + cfg_offset_y, 0, 0, 0, 0);
}


void OnRedraw(void)
{
	Draw();
}

void StopUpdate(void)
{
	DelTimer(&tmr);
}

void AutoUpdate(void)
{
	if (IsGuiOnTop(shell_gui_id))
	{
		DrawSeparateBG(cfg_pos_x, cfg_pos_y, cfg_pos_x + bg->w, cfg_pos_y + bg->h);
		Draw();
		GBS_StartTimerProc(&tmr, TMR_6_SEC / 6 * cfg_update_time, (void*)AutoUpdate);
	}
	else
		StopUpdate();
}

void OnFocus(void)
{
	GBS_StartTimerProc(&tmr, TMR_6_SEC / 6 * cfg_update_time, (void*)AutoUpdate);
}

int LoadPlgGraphics(void)
{
	FSTATS fs;
	unsigned int err;
	char path[128];
	
	sprintf(path, "%s%s%s", img_dir, "dclock\\", "bg.png");
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	bg = CreateIMGHDRFromPngFile(path, 0);
	
	for (int i = 0; i < 10; i++)
	{
		sprintf(path, "%s%s%d%s", img_dir, "dclock\\", i, ".png");
		if (GetFileStats(path, &fs, &err) == -1) return -1;
		digits[i] = CreateIMGHDRFromPngFile(path, 0);
	}
	return 0;
}

void Destroy(void)
{
	StopUpdate();
	if (ws)
		FreeWS(ws);
	FreeIMGHDR(bg);
	for (int i = 0; i < 10; i++)
		FreeIMGHDR(digits[i]);
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
	//чтение конфига
	char bcfg[128];
	sprintf(bcfg, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(bcfg);
	
	plg->Destroy   = (void*)Destroy;
	plg->OnRedraw  = (void*)OnRedraw;
	plg->OnFocus   = (void*)OnFocus;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;

	plg->StartUpdate = (void*)AutoUpdate;
	plg->StopUpdate  = (void*)StopUpdate;
	
	if(LoadPlgGraphics() == -1) return -1;
	
	ws = AllocWS(16);
	
	plg->desk_id     = cfg_desk_id;
	desk_id_ptr = &plg->desk_id;
	
	return 0;
}
