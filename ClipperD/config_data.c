#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)
	
	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Maximum history items", 0, 100};
	__root const unsigned int cfg_max_history = 10;
		
	__root const CFG_HDR cfghdr2 = {CFG_LEVEL, "KeyHook", 1, 0};
		
		__root const CFG_HDR cfghdr2_1 = {CFG_CHECKBOX, "Enable KeyHook", 0, 2};
		__root const unsigned int cfg_enable_keyhook = 1;
		
		__root const CFG_HDR cfghdr2_2 = {CFG_KEYCODE, "Key", 0, 100};
		__root const unsigned int cfg_key = GREEN_BUTTON;
		
		__root const CFG_HDR cfghdr2_3 = {CFG_CBOX, "Activation style", 0, 2};
		__root const unsigned int cfg_style = 0;
		__root const CFG_CBOX_ITEM cfgcbox1_3[2] = {"Key Down", "Long press"};
		
		__root const CFG_HDR cfghdr2_4 = {CFG_CHECKBOX, "Enable of IdleScreen", 0, 2};
		__root const unsigned int cfg_enable_of_idlescreen = 0;
		
	__root const CFG_HDR cfghdr2_5 = {CFG_LEVEL, "", 0, 0};
	
	__root const CFG_HDR cfghdr3_1 = {CFG_LEVEL, "Files", 1, 0};
	
		__root const CFG_HDR cfghdr3_2 = {CFG_STR_UTF8, "Path to lang.txt", 0, 127};
		__root const char cfg_lang_path[128] = "0:\\Zbin\\Utilities\\ClipperD\\lang.txt";
	
		__root const CFG_HDR cfghdr3_3 = {CFG_STR_UTF8, "Path to header icon", 0, 127};
		__root const char cfg_icon_path[128] = "0:\\Zbin\\Utilities\\ClipperD\\icon.png";
		
	__root const CFG_HDR cfghdr3_4 = {CFG_LEVEL, "", 0, 0};
	
__CFG_END(0)
