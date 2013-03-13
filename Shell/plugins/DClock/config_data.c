#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Desktop ID", 0, 100};
	__root const unsigned int cfg_desk_id = 1;

	__root const CFG_HDR cfghdr1 = {CFG_UINT, "Update time(sec)", 0, 10};
	__root const unsigned int cfg_update_time = 2;
	
	__root const CFG_HDR cfghdr2={CFG_COORDINATES, "Background position", 0, 0};
	__root const unsigned int cfg_pos_x = 25;
	__root const unsigned int cfg_pos_y = 65;
	
	__root const CFG_HDR cfghdr3 = {CFG_UINT, "Hours offset X", 0, 240};
	__root const unsigned int cfg_h_offset_x = 8;
	
	__root const CFG_HDR cfghdr4 = {CFG_UINT, "Minutes offset X", 0, 240};
	__root const unsigned int cfg_m_offset_x = 105;
	
	__root const CFG_HDR cfghdr5 = {CFG_UINT, "Digits offset Y", 0, 320};
	__root const unsigned int cfg_offset_y = 15;
	
	
__CFG_END(0)
