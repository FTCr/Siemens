#include <swilib.h>

int InitLang(const char *path, char ***lgp)
{
	FSTATS fs;
	unsigned int err;

	if (GetFileStats(path, &fs, &err) == -1) return -1;

	int fp = _open(path, A_ReadOnly + A_BIN, P_READ, &err);
	if (fp == -1) return -1;

	char *buffer = malloc(fs.size + 1);
	buffer[fs.size] = '\0';
	_read(fp, buffer, fs.size, &err);
	char tmp[128];
	unsigned int pos = 0, len = 0, total = 0;

	char **tmp_lgp = NULL;
	while (pos <= fs.size)
	{
		if (buffer[pos] == '\n' || buffer[pos] == '\r' || buffer[pos] == '\0')
		{
			if (len)
			{
				tmp_lgp = realloc(tmp_lgp, sizeof(char*) * (total + 2));
				tmp_lgp[total]     = malloc(len + 1);
				tmp_lgp[total + 1] = NULL;

				memcpy(tmp_lgp[total], tmp, len);
				*((char*)(tmp_lgp[total++]) + len) = '\0';

				len = 0;
				*lgp=tmp_lgp;
			}
		}
		else
			tmp[len++] = buffer[pos];
				pos++;
	}
	mfree(buffer);
	_close(fp, &err);
	return total;
}

void FreeLang(char ***lgp)
{
	char **ptr = *lgp;
	if (ptr)
	{
		for (unsigned int i = 0; ptr[i] != NULL; i++)
			mfree(ptr[i]);
		mfree(ptr);
	}
}
