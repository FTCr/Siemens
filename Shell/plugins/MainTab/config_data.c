#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Desktop ID", 0, 100};
	__root const unsigned int cfg_desk_id = 1;

	__root const CFG_HDR cfghdr0_1={CFG_CHECKBOX,"Use left soft",0,0};
	__root const unsigned int cfg_use_ls = 1;

	__root const CFG_HDR cfghdr0_2 = {CFG_STR_WIN1251, "Left soft shortcut", 0, 31};
	__root const char cfg_ls_func[32]="DEVICE_INBOX";

	__root const CFG_HDR cfghdr0_3={CFG_CHECKBOX,"Use right soft",0,0};
	__root const unsigned int cfg_use_rs = 0;

	__root const CFG_HDR cfghdr0_4 = {CFG_STR_WIN1251, "Right soft func shortcut", 0, 31};
	__root const char cfg_rs_func[32]="";
	
	__root const CFG_HDR cfghdr0_5={CFG_CHECKBOX,"Use enter button",0,0};
	__root const unsigned int cfg_use_enter = 1;
	
	__root const CFG_HDR cfghdr0_6 = {CFG_STR_WIN1251, "Enter button func shortcut", 0, 31};
	__root const char cfg_enter_func[32]="MAIN_MENU";
	
__CFG_END(0)
