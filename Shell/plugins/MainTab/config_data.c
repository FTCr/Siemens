#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Desktop ID", 0, 100};
	__root const unsigned int cfg_desk_id = 1;

	__root const CFG_HDR cfghdr0_1={CFG_CHECKBOX,"Use left soft",0,0};
	__root const unsigned int cfg_use_ls = 1;

	__root const CFG_HDR cfghdr0_2 = {CFG_STR_WIN1251, "Left soft func", 0, 63};
	__root const char cfg_ls_func[64]="DEVICE_INBOX";

	__root const CFG_HDR cfghdr0_3={CFG_CHECKBOX,"Use right soft",0,0};
	__root const unsigned int cfg_use_rs = 1;

	__root const CFG_HDR cfghdr0_4 = {CFG_STR_WIN1251, "Right soft func", 0, 63};
	__root const char cfg_rs_func[64]="MAIN_MENU";

__CFG_END(0)
