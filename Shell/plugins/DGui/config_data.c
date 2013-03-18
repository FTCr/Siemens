#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Workspace ID", 0, 100};
	__root const unsigned int cfg_desk_id = 0;

	__root const CFG_HDR cfghdr1 = {CFG_UINT, "Lines, max", 0, 50};
	__root const unsigned int cfg_max_num_lines = 5;
	
__CFG_END(0)
