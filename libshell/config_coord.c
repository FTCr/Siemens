#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(3)

	__root const CFG_HDR coordhdr0 = {CFG_UINT, "Horizontal header indent", 0, 100};
	__root const unsigned int cfg_coord_head_off_x = 5;

	__root const CFG_HDR coordhdr2 = {CFG_UINT, "Vertical softkeys indent", 0, 100};
	__root const unsigned int cfg_coord_soft_off_x = 10;
	
	__root const CFG_HDR coordhdr3 = {CFG_UINT, "Vertical alignment point", 0, 320};
	__root const unsigned int cfg_coord_h_mid = 150;
	
	__root const CFG_HDR coordhdr4 = {CFG_UINT, "Vertical menu 1 indent", 0, 320};
	__root const unsigned int cfg_coord_menu1_off_y = 1;
	
	__root const CFG_HDR coordhdr5 = {CFG_UINT, "Menu 1 max items", 3, 25};
	__root const unsigned int cfg_coord_max_menu1_items = 6;
	
	__root const CFG_HDR coordhdr6 = {CFG_UINT, "Vertical menu 2 indent", 0, 320};
	__root const unsigned int cfg_coord_menu2_off_y = 3;
	
	__root const CFG_HDR coordhdr7 = {CFG_UINT, "Menu 2 max items", 3, 25};
	__root const unsigned int cfg_coord_max_menu2_items = 7;
	
__CFG_END(3)
