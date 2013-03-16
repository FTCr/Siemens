#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Desktop ID", 0, 100};
	__root const unsigned int cfg_desk_id = 1;
	
	__root const CFG_HDR cfghdr1 = {CFG_CBOX,"Key", 0, 2};
	__root const unsigned int cfg_key = 1;
	__root const CFG_CBOX_ITEM cfgcbox0[2] = {"LEFT_SOFT", "RIGHT_SOFT"};
	
	__root const CFG_HDR cfghdr2={CFG_COORDINATES, "Y position", 0, 0};
	__root const unsigned int cfg_pos_x = 0;
	__root const unsigned int cfg_pos_y = 140;
	
__CFG_END(0)
