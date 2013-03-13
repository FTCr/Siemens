#include <swilib.h>
#include "config_col.h"
#include "config_font.h"
#include "skin_utils.h"

const char *GetColor(const unsigned int color_id)
{
	switch (color_id)
	{
		case colorHeader:   return cfg_col_header;
		case colorSoftkeys: return cfg_col_softkeys;
		case colorMain:     return cfg_col_main;
	}
	return NULL;
}

unsigned int GetFontFromCfg(const unsigned int id)
{
	switch (id)
	{
		case 0:  return FONT_SMALL;
		case 1:  return FONT_SMALL_BOLD;
		case 2:  return FONT_MEDIUM;
		case 3:  return FONT_MEDIUM_BOLD;
		case 4:  return FONT_LARGE;
		case 5:  return FONT_LARGE_BOLD;
		default: return FONT_SMALL;
	}
}

unsigned int GetFont(const unsigned int font_id)
{
	switch (font_id)
	{
		case fontHeader1:  return GetFontFromCfg(cfg_font_header1);
		case fontHeader2:  return GetFontFromCfg(cfg_font_header2);
		case fontMain1:    return GetFontFromCfg(cfg_font_main1);
		case fontMain2:    return GetFontFromCfg(cfg_font_main2);
		case fontMain3:    return GetFontFromCfg(cfg_font_main3);
		case fontSoftkeys: return GetFontFromCfg(cfg_font_softkeys);
		default:           return FONT_SMALL;
	}
}
