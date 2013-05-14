#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_KEYCODE, "Key", 0, 100};
	__root const unsigned int cfg_key = RIGHT_SOFT;
		
	__root const CFG_HDR cfghdr0_1 = {CFG_STR_UTF8, "Path to lang.txt", 0, 127};
	__root const char cfg_lang_path[128] = "0:\\Zbin\\Utilities\\TurnOffD\\lang.txt";
	
	__root const CFG_HDR cfghdr0_2 = {CFG_LEVEL, "Icons", 1, 0};
	
		__root const CFG_HDR cfghdr0_2_1 = {CFG_CHECKBOX, "Enable", 0, 2};
		__root const unsigned int cfg_enable = 1;
		
		__root const CFG_HDR cfghdr0_2_2 = {CFG_STR_UTF8, "Path to shutdown icon", 0, 127};
		__root const char cfg_icon_shutdown_path[128] = "0:\\Zbin\\Utilities\\TurnOffD\\Icons\\shutdown.png";
		
		__root const CFG_HDR cfghdr0_2_3 = {CFG_STR_UTF8, "Path to reboot icon", 0, 127};
		__root const char cfg_icon_reboot_path[128] = "0:\\Zbin\\Utilities\\TurnOffD\\Icons\\reboot.png";
	
		__root const CFG_HDR cfghdr0_2_4 = {CFG_STR_UTF8, "Path to lock icon", 0, 127};
		__root const char cfg_icon_lock_path[128] = "0:\\Zbin\\Utilities\\TurnOffD\\Icons\\lock.png";
	
	__root const CFG_HDR cfghdr0_2_5 = {CFG_LEVEL, "", 0, 0};
	
__CFG_END(0)
