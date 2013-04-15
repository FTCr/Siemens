#ifndef _LIBSIEMENS_STRINGS_H_
	#define _LIBSIEMENS_STRINGS_H_

int GetExtByPath(char *dest, const char *path);
int GetFileNameWithoutExt(char *dest, const char *fname);
int GetDirByPath(char *dest, const char *path);
void utf8_2fname(char *dest, const char *source);
//преобразование строки в нижний регистр
int str2lower(char *dest, const char *src);
//аналого strcmp, но без чувствительности к регистру
int strcmp_nocase(const char *s, const char *d);

#endif
