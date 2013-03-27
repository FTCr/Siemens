#include <swilib.h>
#include "../../../libsiemens/other.h"
#include "../../../libsiemens/graphics.h"
#include "../../../libshell/plugins.h"
#include "../../../libshell/other.h"
#include "../../../libshell/skin_utils.h"
#include "../../../libshell/graphics.h"
#include "conf_loader.h"
#include "config_data.h"

//создать структуру и инициализировать массив структур при старте? //нахуй надо....

GBSTMR tmr_freeram, tmr_cpuload, tmr_akkucapa;
WSHDR *ws;

unsigned int *desk_id_ptr;

void DrawData(const int is_draw_bg, const int cfg_x, const int cfg_y, /*const int cfg_align,*/ const char *cfg_string, unsigned int data)
{
	wsprintf(ws, cfg_string, data);
	unsigned int align;
	unsigned int font = GetFont(fontMain1);
	unsigned int width = GetWidthWS(ws, font);
	unsigned int height = GetFontYSIZE(font);
	
	/*switch (cfg_align)
	{
		case 0: align = TEXT_ALIGNLEFT; break;
		case 1: align = TEXT_ALIGNMIDDLE; break;
		case 2: align = TEXT_ALIGNRIGHT; break;
	}*/
	
	if (is_draw_bg)
		DrawSeparateBG(cfg_x, cfg_y, cfg_x + width, cfg_y + height);
	
	DrawString(ws, cfg_x, cfg_y, cfg_x + width, cfg_y + height, font, TEXT_ALIGNLEFT, GetColor(colorMain), GetPaletteAdrByColorIndex(23));
}

void DrawFreeRam(const int is_draw_bg)
{
	unsigned int denominator = 1;
	if (cfg_freeram_show_in == 1)
		denominator = 1024;
	else if (cfg_freeram_show_in == 2)
		denominator = 1024 * 1024;
	DrawData(is_draw_bg, cfg_freeram_pos_x, cfg_freeram_pos_y, /*cfg_freeram_align,*/ cfg_freeram_string, GetFreeRamAvail()/denominator);
}

void DrawCPULoad(const int is_draw_bg)
{
	DrawData(is_draw_bg, cfg_cpuload_pos_x, cfg_cpuload_pos_y, cfg_cpuload_string, GetCPULoad());
}

void DrawAkkuCapa(const int is_draw_bg)
{
	DrawData(is_draw_bg, cfg_akkucapa_pos_x, cfg_akkucapa_pos_y, cfg_akkucapa_string, *RamCap());
}

void OnRedraw(void)
{
	if (cfg_freeram_apply)
		DrawFreeRam(0);
	if (cfg_cpuload_apply)
		DrawCPULoad(0);
	if (cfg_akkucapa_apply)
		DrawAkkuCapa(0);
}

void AutoUpdate_FreeRam(void)
{
	DrawFreeRam(1);
	GBS_StartTimerProc(&tmr_freeram, TMR_6_SEC / 6 * cfg_freeram_update_time, (void*)AutoUpdate_FreeRam);
}

void AutoUpdate_CPULoad(void)
{
	DrawCPULoad(1);
	GBS_StartTimerProc(&tmr_cpuload, TMR_6_SEC / 6 * cfg_cpuload_update_time, (void*)AutoUpdate_CPULoad);
}

void AutoUpdate_AkkuCapa(void)
{
	DrawAkkuCapa(1);
	GBS_StartTimerProc(&tmr_akkucapa, TMR_6_SEC / 6 * cfg_akkucapa_update_time, (void*)AutoUpdate_AkkuCapa);
}

void DelTimers(void)
{
	DelTimer(&tmr_freeram);
	DelTimer(&tmr_cpuload);
	DelTimer(&tmr_akkucapa);
}

void OnFocus(void)
{
	if (cfg_freeram_apply)
		GBS_StartTimerProc(&tmr_freeram, TMR_6_SEC / 6 * cfg_freeram_update_time, (void*)AutoUpdate_FreeRam);
	if (cfg_cpuload_apply)
		GBS_StartTimerProc(&tmr_cpuload, TMR_6_SEC / 6 * cfg_cpuload_update_time, (void*)AutoUpdate_CPULoad);
	if (cfg_akkucapa_apply)
		GBS_StartTimerProc(&tmr_akkucapa, TMR_6_SEC / 6 * cfg_akkucapa_update_time, (void*)AutoUpdate_CPULoad);
}

void Destroy(void)
{
	DelTimers();
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
	//чтение конфига
	char bcfg[128];
	sprintf(bcfg, "%s%s%s", conf_dir, plg->fname, ".bcfg");
	InitConfig(bcfg);
	
	plg->OnRedraw  = (void*)OnRedraw;
	plg->OnFocus   = (void*)OnFocus;
	plg->OnUnFocus = (void*)DelTimers;
	plg->Destroy   = (void*)Destroy;
	plg->OnMessage = (void(*)(CSM_RAM*, GBS_MSG*))OnMessage;
	
	ws = AllocWS(64);
	
	plg->desk_id = cfg_desk_id;
	desk_id_ptr  = &plg->desk_id;
	
	return 0;
}
