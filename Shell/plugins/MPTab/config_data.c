#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_UINT, "Desktop ID", 0, 100};
	__root const unsigned int cfg_desk_id = 5;

	__root const CFG_HDR cfghdr1 = {CFG_UINT, "Update time (sec)", 0, 60};
	__root const unsigned int cfg_update_time = 1;

	__root const CFG_HDR cfghdr2 = {CFG_STR_WIN1251, "Mediaplayer CSM addr", 1, 8};
	__root const char cfg_mp_csm_adr[9] = "A087109C";

	__root const CFG_HDR cfghdr3 = {CFG_STR_UTF8, "Covers directory", 0, 127};
	__root const char cfg_covers_directory[128]="0:\\Pictures\\Covers\\";

	__root const CFG_HDR cfghdr4={CFG_LEVEL, "Coordinaties", 1, 0};

		__root const CFG_HDR cfghdr4_1 = {CFG_RECT, "Cover", 0, 0};
		__root const RECT cfg_rc_cover = {55,65,185,195};

		__root const CFG_HDR cfghdr4_2 = {CFG_UINT, "Title Y", 0, 320};
		__root const unsigned int cfg_y_title = 205;

		__root const CFG_HDR cfghdr4_3 = {CFG_UINT, "Artist Y", 0, 320};
		__root const unsigned int cfg_y_artist = 230;

		__root const CFG_HDR cfghdr4_4 = {CFG_UINT, "File name Y", 0, 320};
		__root const unsigned int cfg_y_fname = 218;

		__root const CFG_HDR cfghdr4_5={CFG_COORDINATES, "Volume", 0, 0};
		__root const unsigned int cfg_x_volume = 21;
		__root const unsigned int cfg_y_volume = 122;

		__root const CFG_HDR cfghdr4_6={CFG_COORDINATES, "VolUp indicator", 0, 0};
		__root const unsigned int cfg_x_vol_up = 9;
		__root const unsigned int cfg_y_vol_up = 73;

		__root const CFG_HDR cfghdr4_7={CFG_COORDINATES, "VolDown indicator", 0, 0};
		__root const unsigned int cfg_x_vol_down = 9;
		__root const unsigned int cfg_y_vol_down = 153;

		__root const CFG_HDR cfghdr4_8={CFG_COORDINATES, "Prev button", 0, 0};
		__root const unsigned int cfg_x_prev = 203;
		__root const unsigned int cfg_y_prev = 73;

		__root const CFG_HDR cfghdr4_9={CFG_COORDINATES, "Pause\\play\\stop buttons", 0, 0};
		__root const unsigned int cfg_x_pp = 200;
		__root const unsigned int cfg_y_pp = 113;

		__root const CFG_HDR cfghdr4_10={CFG_COORDINATES, "Prev buttons", 0, 0};
		__root const unsigned int cfg_x_next = 203;
		__root const unsigned int cfg_y_next = 158;

	__root const CFG_HDR cfghdr5={CFG_LEVEL, "", 0, 0};

__CFG_END(0)
