#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Workspace ID", 0, 100};
	__root const unsigned int cfg_desk_id = 1;

	__root const CFG_HDR cfghdr1 = {CFG_LEVEL, "FreeRAM", 1, 0};
		
		__root const CFG_HDR cfghdr1_1 = {CFG_CHECKBOX, "Apply", 0, 2};
		__root const unsigned int cfg_freeram_apply  = 1;
		
		__root const CFG_HDR cfghdr1_2 = {CFG_UINT, "Refresh frequency in sec.", 1, 10};
		__root const unsigned int cfg_freeram_update_time = 1;
		
		__root const CFG_HDR cfghdr1_3 = {CFG_COORDINATES, "Position", 0, 0};
		__root const unsigned int cfg_freeram_pos_x = 4;
		__root const unsigned int cfg_freeram_pos_y  = 239;
		
		/*__root const CFG_HDR cfghdr1_4 = {CFG_CBOX, "Align", 0, 3};
		__root const unsigned int cfg_freeram_align = 0;
		__root const CFG_CBOX_ITEM cfgcbox1_4[3] = {"Left", "Middle", "Right"};*/
		
		__root const CFG_HDR cfghdr1_5 = {CFG_STR_UTF8, "String", 0, 31};
		__root const char cfg_freeram_string[32]="FR: %d KB |";
		
		__root const CFG_HDR cfghdr1_6 = {CFG_CBOX, "Show in", 0, 3};
		__root const unsigned int cfg_freeram_show_in = 1;
		__root const CFG_CBOX_ITEM cfgcbox1_6[3] = {"B", "KB", "MB"};
		
	__root const CFG_HDR cfghdr1_7 = {CFG_LEVEL, "", 0, 0};
	
	__root const CFG_HDR cfghdr2 = {CFG_LEVEL, "CPU load", 1, 0};
		
		__root const CFG_HDR cfghdr2_1 = {CFG_CHECKBOX, "Apply", 0, 2};
		__root const unsigned int cfg_cpuload_apply  = 1;
		
		__root const CFG_HDR cfghdr2_2 = {CFG_UINT, "Refresh frequency in sec.", 1, 10};
		__root const unsigned int cfg_cpuload_update_time = 1;
		
		__root const CFG_HDR cfghdr2_3 = {CFG_COORDINATES, "Position", 0, 0};
		__root const unsigned int cfg_cpuload_pos_x = 87;
		__root const unsigned int cfg_cpuload_pos_y  = 239;
		
		/*__root const CFG_HDR cfghdr2_4 = {CFG_CBOX, "Align", 0, 3};
		__root const unsigned int cfg_freeram_align = 0;
		__root const CFG_CBOX_ITEM cfgcbox2_4[3] = {"Left", "Middle", "Right"};*/
		
		__root const CFG_HDR cfghdr2_5 = {CFG_STR_UTF8, "String", 0, 31};
		__root const char cfg_cpuload_string[32]="CPU load: %d\% |";
		
	__root const CFG_HDR cfghdr2_7 = {CFG_LEVEL, "", 0, 0};
	
	__root const CFG_HDR cfghdr3 = {CFG_LEVEL, "Akku capacity", 1, 0};
		
		__root const CFG_HDR cfghdr3_1 = {CFG_CHECKBOX, "Apply", 0, 2};
		__root const unsigned int cfg_akkucapa_apply  = 1;
		
		__root const CFG_HDR cfghdr3_2 = {CFG_UINT, "Refresh frequency in sec.", 1, 10};
		__root const unsigned int cfg_akkucapa_update_time = 1;
		
		__root const CFG_HDR cfghdr3_3 = {CFG_COORDINATES, "Position", 0, 0};
		__root const unsigned int cfg_akkucapa_pos_x = 188;
		__root const unsigned int cfg_akkucapa_pos_y  = 239;
		
		/*__root const CFG_HDR cfghdr3_4 = {CFG_CBOX, "Align", 0, 3};
		__root const unsigned int cfg_freeram_align = 0;
		__root const CFG_CBOX_ITEM cfgcbox3_4[3] = {"Left", "Middle", "Right"};*/
		
		__root const CFG_HDR cfghdr3_5 = {CFG_STR_UTF8, "String", 0, 31};
		__root const char cfg_akkucapa_string[32]="C: %d\%";
		
	__root const CFG_HDR cfghdr3_7 = {CFG_LEVEL, "", 0, 0};
	
__CFG_END(0)
