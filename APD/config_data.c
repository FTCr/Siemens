#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_STR_UTF8, "Music directory", 0, 127};
	__root const char cfg_mus_dir[128] = "4:\\Music\\";
	
	__root const CFG_HDR cfghdr1 = {CFG_STR_UTF8, "Playlists directory", 0, 127};
	__root const char cfg_pls_dir[128] = "4:\\Playlists\\";
	
__CFG_END(0)
