#ifndef _LIBS_STRING_UTILS_H_
	#define _LIBS_STRING_UTILS_H_

int str2lower(char *dest, const char *src); //преобразование строки в нижний регистр
int strcmp_nocase(const char *s, const char *d); //аналог strcmp, но без чувствительности к регистру

#endif
