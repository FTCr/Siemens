#ifndef _LIBSIEMENS_STRINGS_H_
	#define _LIBSIEMENS_STRINGS_H_

//преобразование строки в нижний регистр
int str2lower(char *dest, const char *src);
//аналого strcmp, но без чувствительности к регистру
int strcmp_nocase(const char *s, const char *d);

#endif
