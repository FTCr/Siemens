#include <swilib.h>

//преобразование строки в нижний регистр
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
