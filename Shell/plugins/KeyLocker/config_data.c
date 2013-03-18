#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Workspace ID", 0, 100};
	__root const unsigned int cfg_desk_id = 0;

	__root const CFG_HDR cfghdr1 = {CFG_KEYCODE, "Keyboard lock button", 0, 99};
	__root const unsigned int cfg_key = '#';

	__root const CFG_HDR cfghdr2 = {CFG_CBOX,"Lock type", 0, 3};
	__root const unsigned int cfg_type_block = 2;
	__root const CFG_CBOX_ITEM cfgcbox0[3] = {"KEY_DOWN", "LONG_PRESS", "Double press"};

	__root const CFG_HDR cfghdr3 = {CFG_UINT, "Double press waiting interval", 10, 200};
	__root const unsigned int cfg_wait_time = 65;

__CFG_END(0)
