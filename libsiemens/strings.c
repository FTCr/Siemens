#include <swilib.h>

int GetExtByPath(char *dest, const char *path)
{
	char *ptr = strrchr(path, '.');
	if (ptr == NULL) return -1;
	ptr++;
	strcpy(dest, ptr);
	return 1;
}

int GetFileNameWithoutExtByFileName(char *dest, const char *fname)
{
	char *ptr = strrchr(fname, '.');
	if (ptr == NULL) return -1;
	int len = ptr - fname;
	strncpy(dest, fname, len);
	dest[len] = '\0';
	return 1;
}

int GetDirByPath(char *dest, const char *path)
{
	char *ptr = strrchr(path, '\\');
	if (ptr == NULL) return -1;
	int len = ptr - path;
	strncpy(dest, path, ptr - path);
	dest[len] = '\0';
	return 1;
}

int GetFileNameByPath(char *dest, const char *path)
{
	char *ptr = strrchr(path, '\\');
	if (ptr == NULL) return -1;
	strcpy(dest, ptr + sizeof(char));
	return 1;
}

int GetFileNameWithoutExtByPath(char *dest, const char *path)
{
	char *ptr1 = strrchr(path, '\\');
	char *ptr2 = strrchr(path, '.');
	
	if (ptr1 == NULL || ptr2 == NULL) return -1;
	
	ptr1++;
	
	int len = ptr2 - ptr1;
	strncpy(dest, ptr1, len);
	dest[len] = '\0';
	return 1;
}

void utf8_2fname(char *dest, const char *source)
{
	int len = strlen(source) + 16;
	WSHDR *ws = AllocWS(len);
	utf8_2ws(ws, source, len);
	ws_2str(ws, dest, len);
	FreeWS(ws);
}

int str2lower(char *dest, const char *src)
{
	unsigned int len = strlen(src);
	if (!len) return -1;
	
	char sym;
	unsigned int i = 0;
	while(i != len)
	{
		sym = src[i];
		if (src[i] >= 'A' && src[i] <= 'Z')
		{
			dest[i] = src[i] + ('a' - 'A');
		}
		else if (src[i] >= 0x90 && src[i] <= 0xAF)
		{
			dest[i] = src[i] + (0xB0 - 0x90);
		}
		else
		{
			dest[i] = src[i];
		}
		i++;
	}
	dest[len] = '\0';
	return i;
}

int strcmp_nocase(const char *s, const char *d)
{
	int cs;
	int ds;
	do
	{
		cs = *s++;
		if (cs&0x40) cs &= 0xDF;
		ds = *d++;
		if (ds & 0x40) ds &= 0xDF;
		cs -= ds;
		if (cs) break;
	}
	while(ds);
	return(cs);
}

int char16to8(int c)
{
	if (c < 0x400) return c;
	c -= 0x400;
	if (c < 16)
	{
		if (c == 1) c = 0;
		else if (c == 4) c = 2;
		else if (c == 6) c = 10;
		else return c;
	}
	else if (c > 79)
	{
		if (c == 0x51) c = 16;
		else if (c == 0x54) c = 18;
		else if (c == 0x56) c = 11;
		else if (c == 0x57) c = 23;
		else return c;
	}
	else c += 8;
	c += 168;
	return c;
}

int ws2ascii(char *dest, WSHDR *ws)
{
  unsigned int sWs = ws->wsbody[0];
  unsigned int p = 0;
  unsigned int cWs;
  while(p < sWs)
  {
    cWs = ws->wsbody[p + 1];
    dest[p] = char16to8(cWs);
    p++;
  }
  dest[p] = 0;
  return p;
}