#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_STR_UTF8, "Music directory", 0, 127};
	__root const char cfg_mus_dir[128] = "4:\\Music\\";
	
	__root const CFG_HDR cfghdr1 = {CFG_STR_UTF8, "Playlists directory", 0, 127};
	__root const char cfg_pls_dir[128] = "4:\\Playlists\\";
	
	__root const CFG_HDR cfghdr2 = {CFG_LEVEL, "IconBar", 1, 0};
		
		__root const CFG_HDR cfghdr2_1 = {CFG_CHECKBOX, "Enable IconBar", 0, 2};
		__root const unsigned int cfg_enable_iconbar = 1;
		
		__root const CFG_HDR cfghdr2_2 = {CFG_UINT, "Play icon", 0, 65535};
		__root const unsigned int cfg_play_icon = 518;
		
		__root const CFG_HDR cfghdr2_3 = {CFG_UINT, "Stop icon", 0, 65535};
		__root const unsigned int cfg_stop_icon = 3000;
		
		__root const CFG_HDR cfghdr2_4 = {CFG_UINT, "Pause icon", 0, 65535};
		__root const unsigned int cfg_pause_icon = 519;
		
		__root const CFG_HDR cfghdr2_5 = {CFG_UINT, "Forward icon", 0, 65535};
		__root const unsigned int cfg_forward_icon = 3001;
		
		__root const CFG_HDR cfghdr2_6 = {CFG_UINT, "Rewind icon", 0, 65535};
		__root const unsigned int cfg_rewind_icon = 3002;
		
	__root const CFG_HDR cfghdr2_7 = {CFG_LEVEL, "", 0, 0};
	
__CFG_END(0)
