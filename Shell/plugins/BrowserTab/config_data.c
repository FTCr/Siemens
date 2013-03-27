#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Workspace ID", 0, 100};
	__root const int cfg_desk_id = 4;
	
	__root const CFG_HDR cfghdr1 = {CFG_STR_UTF8, "BM bookmarks directory path", 0, 127};
	__root const char cfg_path_bm_bookmark_dir[128] = "0:\\Zbin\\Network\\BalletMini\\Bookmarks\\";
	
	__root const CFG_HDR cfghdr2 = {CFG_STR_UTF8, "BM.elf path", 0, 127};
	__root const char cfg_path_bm[128] = "0:\\Zbin\\Network\\BalletMini\\BalletMini.elf";
	
__CFG_END(0)
