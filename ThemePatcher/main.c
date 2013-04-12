#include <swilib.h>
#include <../libsiemens/graphics.h>

void patch(IMGHDR *dest, IMGHDR *src)
{
	mfree(dest->bitmap);
	memcpy(dest, src, sizeof(IMGHDR));
	mfree(src);
}

void patch2(IMGHDR *dest, IMGHDR *src)
{
	mfree(dest->bitmap);
	dest->bitmap = src->bitmap;
}

void main(void)
{
	IMGHDR *img = NULL;
	IMGHDR *img2 = NULL;
	
	//полупрозрачность
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_SELECTION_1LINE);
	img2 = CreateIMGHDRFromPngFile("2:\\Default\\Selection1LineBgImage.png", 0);
	patch(img, img2);
	
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_SELECTION_2LINE);
	img2 = CreateIMGHDRFromPngFile("2:\\Default\\Selection2LineBgImage.png", 0);
	patch(img, img2);
	
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_SELECTION_3LINE);
	img2 = CreateIMGHDRFromPngFile("2:\\Default\\Selection3LineBgImage.png", 0);
	patch(img, img2);

	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_TAB_SELECTED);
	img2 = CreateIMGHDRFromPngFile("2:\\Default\\TabSelectedBgImage.png", 0);
	patch(img, img2);
	
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_TAB_UNSELECTED);
	img2 = CreateIMGHDRFromPngFile("2:\\Default\\TabUnselectedBgImage.png", 0);
	patch(img, img2);
	
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_POPUP_FEEDBACK);
	img2 = CreateIMGHDRFromPngFile("2:\\Default\\PopupFeedbackBgImage.png", 0);
	patch(img, img2);
	
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_POPUP_QUICK_ACCESS_FIELD);
	img2 = CreateIMGHDRFromPngFile("2:\\Default\\PopupQuickAccessFieldBgImage.png", 0);
	patch(img, img2);
	
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_POPUP_SEARCH_LINE);
	img2 = CreateIMGHDRFromPngFile("2:\\Default\\PopupSearchLineBgImage.png", 0);
	patch(img, img2);
	
	//экономим память
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_HEADLINE_FULLSCREEN);
	img2 = GetIMGHDRFromThemeCache(THEME_CACHE_ID_HEADLINE_STANDARD);
	patch2(img, img2);
	
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_BODY_STANDARD);
	img2 = GetIMGHDRFromThemeCache(THEME_CACHE_ID_BODY_TAB);
	patch2(img, img2);
	
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_BOTTOM_STANDARD);
	img2 = GetIMGHDRFromThemeCache(THEME_CACHE_ID_BOTTOM_FULLSCREEN);
	patch2(img, img2);
	
	img  = GetIMGHDRFromThemeCache(THEME_CACHE_ID_STATUSBAR_STANDARD);
	img2 = GetIMGHDRFromThemeCache(THEME_CACHE_ID_STATUSBAR_FULLSCREEN);
	patch2(img, img2);
	
	kill_elf();
}
