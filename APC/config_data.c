#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_CHECKBOX, "Stop playing when you exit", 0, 2};
	__root const unsigned int cfg_stop_playing = 0;

__CFG_END(0)
