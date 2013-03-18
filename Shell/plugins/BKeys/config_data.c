#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Workspace ID", 0, 100};
	__root const unsigned int cfg_desk_id = 0;
	
		//*****************************LEFT_BUTTON**********************************//
		__root const CFG_HDR cfghdr1 = {CFG_LEVEL, "LEFT_BUTTON", 1, 0};
	
			__root const CFG_HDR cfghdr1_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_lb  = 0;
			
			__root const CFG_HDR cfghdr1_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_lb = 0;
			__root const CFG_CBOX_ITEM cfgcbox1_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr1_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_lb = 0;
			__root const CFG_CBOX_ITEM cfgcbox1_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr1_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_lb[128] = "";
	
		__root const CFG_HDR cfghdr1_5 = {CFG_LEVEL, "", 0, 0};
	
		//*****************************RIGHT_BUTTON********************************//
		
		__root const CFG_HDR cfghdr2 = {CFG_LEVEL, "RIGHT_BUTTON", 1, 0};
	
			__root const CFG_HDR cfghdr2_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_rb  = 0;
			
			__root const CFG_HDR cfghdr2_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_rb = 0;
			__root const CFG_CBOX_ITEM cfgcbox2_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr2_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_rb = 0;
			__root const CFG_CBOX_ITEM cfgcbox2_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr2_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_rb[128] = "";
	
		__root const CFG_HDR cfghdr2_5 = {CFG_LEVEL, "", 0, 0};
		
		//**********************************UP_BUTTON*******************************//
		
		__root const CFG_HDR cfghdr3 = {CFG_LEVEL, "UP_BUTTON", 1, 0};
	
			__root const CFG_HDR cfghdr3_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_ub  = 1;
			
			__root const CFG_HDR cfghdr3_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_ub = 0;
			__root const CFG_CBOX_ITEM cfgcbox3_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr3_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_ub = 1;
			__root const CFG_CBOX_ITEM cfgcbox3_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr3_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_ub[128] = "USER_PROFILES";
	
		__root const CFG_HDR cfghdr3_5 = {CFG_LEVEL, "", 0, 0};
		
		//**********************************DOWN_BUTTON*******************************//
		
		__root const CFG_HDR cfghdr4 = {CFG_LEVEL, "DOWN_BUTTON", 1, 0};
	
			__root const CFG_HDR cfghdr4_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_db  = 1;
			
			__root const CFG_HDR cfghdr4_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_db = 0;
			__root const CFG_CBOX_ITEM cfgcbox4_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr4_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_db = 1;
			__root const CFG_CBOX_ITEM cfgcbox4_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr4_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_db[128] = "CONTACTS_LIST";
	
		__root const CFG_HDR cfghdr4_5 = {CFG_LEVEL, "", 0, 0};
		
		//*****************************GREEN_BUTTON********************************//
		
		__root const CFG_HDR cfghdr5 = {CFG_LEVEL, "GREEN_BUTTON", 1, 0};
	
			__root const CFG_HDR cfghdr5_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_gb  = 1;
			
			__root const CFG_HDR cfghdr5_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_gb = 0;
			__root const CFG_CBOX_ITEM cfgcbox5_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr5_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_gb = 1;
			__root const CFG_CBOX_ITEM cfgcbox5_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr5_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_gb[128] = "REC_DIALED_LIST";
	
		__root const CFG_HDR cfghdr5_5 = {CFG_LEVEL, "", 0, 0};
		
		//******************************1*****************************************//
		
		__root const CFG_HDR cfghdr6 = {CFG_LEVEL, "1", 1, 0};
	
			__root const CFG_HDR cfghdr6_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_1  = 0;
			
			__root const CFG_HDR cfghdr6_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_1 = 0;
			__root const CFG_CBOX_ITEM cfgcbox6_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr6_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_1 = 0;
			__root const CFG_CBOX_ITEM cfgcbox6_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr6_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_1[128] = "";
	
		__root const CFG_HDR cfghdr6_5 = {CFG_LEVEL, "", 0, 0};
		
		//******************************2*****************************************//
		
		__root const CFG_HDR cfghdr7 = {CFG_LEVEL, "2", 1, 0};
	
			__root const CFG_HDR cfghdr7_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_2  = 0;
			
			__root const CFG_HDR cfghdr7_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_2 = 0;
			__root const CFG_CBOX_ITEM cfgcbox7_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr7_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_2 = 0;
			__root const CFG_CBOX_ITEM cfgcbox7_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr7_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_2[128] = "";
	
		__root const CFG_HDR cfghdr7_5 = {CFG_LEVEL, "", 0, 0};
		
		//******************************3*****************************************//
		
		__root const CFG_HDR cfghdr8 = {CFG_LEVEL, "3", 1, 0};
	
			__root const CFG_HDR cfghdr8_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_3  = 1;
			
			__root const CFG_HDR cfghdr8_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_3 = 1;
			__root const CFG_CBOX_ITEM cfgcbox8_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr8_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_3 = 1;
			__root const CFG_CBOX_ITEM cfgcbox8_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr8_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_3[128] = "DEVELOPER_MENU";
	
		__root const CFG_HDR cfghdr8_5 = {CFG_LEVEL, "", 0, 0};
		
		//********************************4****************************************//
		
		__root const CFG_HDR cfghdr9 = {CFG_LEVEL, "4", 1, 0};
	
			__root const CFG_HDR cfghdr9_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_4  = 0;
			
			__root const CFG_HDR cfghdr9_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_4 = 0;
			__root const CFG_CBOX_ITEM cfgcbox9_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr9_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_4 = 0;
			__root const CFG_CBOX_ITEM cfgcbox9_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr9_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_4[128] = "";
	
		__root const CFG_HDR cfghdr9_5 = {CFG_LEVEL, "", 0, 0};
		

		//********************************5****************************************//
		
		__root const CFG_HDR cfghdr10 = {CFG_LEVEL, "5", 1, 0};
	
			__root const CFG_HDR cfghdr10_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_5  = 1;
			
			__root const CFG_HDR cfghdr10_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_5 = 1;
			__root const CFG_CBOX_ITEM cfgcbox10_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr10_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_5 = 1;
			__root const CFG_CBOX_ITEM cfgcbox10_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr10_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_5[128] = "BLUETOOTH";
	
		__root const CFG_HDR cfghdr10_5 = {CFG_LEVEL, "", 0, 0};
		
		//********************************6****************************************//
		
		__root const CFG_HDR cfghdr11 = {CFG_LEVEL, "6", 1, 0};
	
			__root const CFG_HDR cfghdr11_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_6  = 0;
			
			__root const CFG_HDR cfghdr11_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_6 = 0;
			__root const CFG_CBOX_ITEM cfgcbox11_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr11_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_6 = 0;
			__root const CFG_CBOX_ITEM cfgcbox11_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr11_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_6[128] = "";
	
		__root const CFG_HDR cfghdr11_5 = {CFG_LEVEL, "", 0, 0};


		//********************************7****************************************//
		
		__root const CFG_HDR cfghdr12 = {CFG_LEVEL, "7", 1, 0};
	
			__root const CFG_HDR cfghdr12_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_7  = 1;
			
			__root const CFG_HDR cfghdr12_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_7 = 1;
			__root const CFG_CBOX_ITEM cfgcbox12_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr12_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_7 = 1;
			__root const CFG_CBOX_ITEM cfgcbox12_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr12_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_7[128] = "GPRS";
	
		__root const CFG_HDR cfghdr12_5 = {CFG_LEVEL, "", 0, 0};

		//********************************8****************************************//
		
		__root const CFG_HDR cfghdr13 = {CFG_LEVEL, "8", 1, 0};
	
			__root const CFG_HDR cfghdr13_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_8  = 0;
			
			__root const CFG_HDR cfghdr13_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_8 = 0;
			__root const CFG_CBOX_ITEM cfgcbox13_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr13_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_8 = 0;
			__root const CFG_CBOX_ITEM cfgcbox13_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr13_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_8[128] = "";
	
		__root const CFG_HDR cfghdr13_5 = {CFG_LEVEL, "", 0, 0};

		//********************************9****************************************//
		
		__root const CFG_HDR cfghdr14 = {CFG_LEVEL, "9", 1, 0};
	
			__root const CFG_HDR cfghdr14_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_9  = 0;
			
			__root const CFG_HDR cfghdr14_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_9 = 0;
			__root const CFG_CBOX_ITEM cfgcbox14_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr14_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_9 = 0;
			__root const CFG_CBOX_ITEM cfgcbox14_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr14_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_9[128] = "";
	
		__root const CFG_HDR cfghdr14_5 = {CFG_LEVEL, "", 0, 0};

		//*************************************************************************//

		__root const CFG_HDR cfghdr15 = {CFG_LEVEL, "*", 1, 0};
	
			__root const CFG_HDR cfghdr15_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_star  = 0;
			
			__root const CFG_HDR cfghdr15_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_star = 0;
			__root const CFG_CBOX_ITEM cfgcbox15_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr15_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_star = 0;
			__root const CFG_CBOX_ITEM cfgcbox15_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr15_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_star[128] = "";
	
		__root const CFG_HDR cfghdr15_5 = {CFG_LEVEL, "", 0, 0};

		//********************************0****************************************//

		__root const CFG_HDR cfghdr16 = {CFG_LEVEL, "0", 1, 0};
	
			__root const CFG_HDR cfghdr16_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_0  = 0;
			
			__root const CFG_HDR cfghdr16_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_0 = 0;
			__root const CFG_CBOX_ITEM cfgcbox16_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr16_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_0 = 0;
			__root const CFG_CBOX_ITEM cfgcbox16_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr16_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_0[128] = "";
	
		__root const CFG_HDR cfghdr16_5 = {CFG_LEVEL, "", 0, 0};

		//********************************0****************************************//

		__root const CFG_HDR cfghdr17 = {CFG_LEVEL, "#", 1, 0};
	
			__root const CFG_HDR cfghdr17_1 = {CFG_CHECKBOX, "Apply?", 0, 2};
			__root const unsigned int cfg_use_lat  = 0;
			
			__root const CFG_HDR cfghdr17_2 = {CFG_CBOX, "Trigger mode", 0, 2};
			__root const unsigned int cfg_style_lat = 0;
			__root const CFG_CBOX_ITEM cfgcbox17_2[2] = {"KEY_DOWN", "LONG_PRESS"};
			
			__root const CFG_HDR cfghdr17_3 = {CFG_CBOX, "Action type", 0, 2};
			__root const unsigned int cfg_action_lat = 0;
			__root const CFG_CBOX_ITEM cfgcbox17_3[2] = {"File", "Shortcut"};
	
			__root const CFG_HDR cfghdr17_4 = {CFG_STR_UTF8, "Action", 0, 127};
			__root const char cfg_execute_lat[128] = "";
	
		__root const CFG_HDR cfghdr17_5 = {CFG_LEVEL, "", 0, 0};


__CFG_END(0)

