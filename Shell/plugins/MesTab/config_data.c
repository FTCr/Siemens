#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Workspace ID", 0, 100};
	__root const unsigned int cfg_desk_id = 3;

	__root const CFG_HDR cfghdr0_1={CFG_COORDINATES, "Icon position", 0, 0};
	__root const unsigned int cfg_x_icon = 202;
	__root const unsigned int cfg_y_icon = 58;

	__root const CFG_HDR cfghdr0_2 = {CFG_STR_UTF8, "MySMSYS path", 0, 127};
	__root const char cfg_mss_path[128]="0:\\Zbin\\Daemons\\MSS.elf";

__CFG_END(0)
