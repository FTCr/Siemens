#include <swilib.h>

int Parse(char *dest, const char *buffer, unsigned int size, const char *keyword)
{
	int ret = -1;
	unsigned int i = 0;
	unsigned int j = 1;
	unsigned int len = strlen(keyword);
	while (i != size)
	{
		if (buffer[i] == keyword[0]) //первый символ подошел
		{
			while (j != len)
			{
				if (buffer[i + 1] != keyword[j]) //не подошло
				{
					j = 1;
					goto END;
				}
				i++;
				j++;
			}
			//всё найдено...
			i += 2;
			j = 0;
			ret = i;
			while(1)
			{
				if (buffer[i] == '\n' || buffer[i] == '\0' || buffer[i] == '\r') break;
				dest[j] = buffer[i];
				j++;
				i++;
			}
			dest[j]='\0';
			return ret;
		}
		END:
			i++;
	}
	return ret;
}

int GetStrDataFromSystemConf(char *dest, const char *path, const char *keyword)
{
	FSTATS fs;
	unsigned int err;
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	
	int fp = _open(path, A_ReadOnly, P_READ, &err);
	if (fp == -1) return -1;
	
	char *buffer = malloc(fs.size);
	if (!_read(fp, buffer, fs.size, &err))
	{
		mfree(buffer);
		_close(fp, &err);
		return -1;
	}
	
	if (Parse(dest, buffer, fs.size - 1, keyword) == -1)
	{
		mfree(buffer);
		_close(fp, &err);
		return -1;
	}
	_close(fp, &err);
	return 1;
}

/*int GetIntegerDataFromSystemConf(int *dest, const char *path, const char *keyword)
{
	FSTATS fs;
	unsigned int err;
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	
	int fp = _open(path, A_ReadOnly, P_READ, &err);
	if (fp == -1) return -1;
	
	char *buffer = malloc(fs.size);
	if (!_read(fp, buffer, fs.size, &err))
	{
		mfree(buffer);
		_close(fp, &err);
		return -1;
	}
	
	char str[64];
	if (Parse(str, buffer, fs.size - 1, keyword) == -1)
	{
		mfree(buffer);
		_close(fp, &err);
		return -1;
	}
	sscanf(str, "%d", dest);
	return 1;
}*/
