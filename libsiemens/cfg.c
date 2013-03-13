#include <swilib.h>

int GetStrDataFromSystemConf(char *dest, const char *path, const char *keyword)
{
	FSTATS fs;
	unsigned int err;
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	
	int fp = _open(path, A_ReadOnly, P_READ, &err);
	if (fp == -1) return -1;
	
	char *buffer = malloc(fs.size);
	if (!_read(fp, buffer, fs.size, &err)) return -1;
	
	unsigned int i = fs.size - 1;
	unsigned int j = 1;
	while(i != 0)
	{
		if (buffer[i] == keyword[0])//первый символ подходит
		{
			while(j != 10)
			{
				if (buffer[i + j] != keyword[j]) //не подошло
				{
					j = 1;
					goto END;
				}
				j++;
			}
			//всё найдено, утекаем от сюда
			i += j + 2;
			j = 0;
			while(1)
			{
				if (buffer[i] == '\n' || buffer[i] == '\0' || buffer[i] == '\r') break;
				dest[j] = buffer[i];
				j++;
				i++;
			}
			dest[j]='\0';
			mfree(buffer);
			_close(fp, &err);
			return 1;
		}
		END:
			i--;
	}
	mfree(buffer);
	_close(fp, &err);
	return -1;
}
