#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Workspace ID", 0, 100};
	__root const unsigned int cfg_desk_id = 1;
	
	__root const CFG_HDR cfghdr1 = {CFG_CBOX, "Activation button", 0, 2};
	__root const unsigned int cfg_key = 1;
	__root const CFG_CBOX_ITEM cfgcbox0[2] = {"Left soft", "Right soft"};
	
__CFG_END(0)
