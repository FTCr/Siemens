#ifndef _LIBSHELL_SKIN_UTILS_H_
	#define _LIBSHELL_SKIN_UTILS_H_

enum
{
	colorHeader,
	colorSoftkeys,
	colorMain
};

enum
{
	fontHeader1,
	fontHeader2,
	fontMain1,
	fontMain2,
	fontMain3,
	fontSoftkeys
};


const char *GetColor(const unsigned int color_id);
unsigned int GetFont(const unsigned int font_id);

#endif
