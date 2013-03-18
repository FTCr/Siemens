#include <swilib.h>
#include <cfg_items.h>

__CFG_BEGIN(1)

__root const CFG_HDR colhdr0 = {CFG_COLOR, "Header colour", 0, 0};
__root const char cfg_col_header[4] = {0xFF,0xFF,0xFF,0x64};

__root const CFG_HDR colhdr1 = {CFG_COLOR, "Softkeys colour", 0, 0};
__root const char cfg_col_softkeys[4] = {0xFF,0xFF,0xFF,0x64};

__root const CFG_HDR colhdr2 = {CFG_COLOR, "Basic colour", 0, 0};
__root const char cfg_col_main[4] = {0x00,0x00,0x00,0x64};

__root const CFG_HDR colhdr3 = {CFG_COLOR, "Basic menu colour", 0, 0};
__root const char cfg_col_menu_main[4] = {0xFF,0xFF,0xFF,0x64};

__root const CFG_HDR colhdr4 = {CFG_COLOR, "Additional menu colour", 0, 0};
__root const char cfg_col_menu_main_add[4] = {0xFF,0xFF,0xFF,0x4A};

__root const CFG_HDR colhdr5 = {CFG_COLOR, "Menu stripes colour", 0, 0};
__root const char cfg_col_menu_line[4] = {0xFF,0xFF,0xFF,0x20};

__CFG_END(1)
