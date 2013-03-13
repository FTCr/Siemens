#include <swilib.h>

unsigned int FindFiles(DIR_ENTRY ***DE, const char *dir, const char *mask)
{
	DIR_ENTRY tmp_de;
	DIR_ENTRY **de = NULL;
	char find[128];
	unsigned int err;
	unsigned int i = 0;
	sprintf(find, "%s%s", dir, mask);
	
	if (FindFirstFile(&tmp_de, find, &err))
	{
		do
		{
			de = realloc(de, sizeof(DIR_ENTRY*) * (i + 2));
			de[i] = malloc(sizeof(DIR_ENTRY));
			de[i + 1] = NULL;
			memcpy(de[i++], &tmp_de, sizeof(DIR_ENTRY));
		} while(FindNextFile(&tmp_de, &err));
		FindClose(&tmp_de, &err);
	}
	*DE = de;
	return i;
}

void DE_Free(DIR_ENTRY ***DE)
{
	DIR_ENTRY **de = *DE;
	unsigned int i = 0;
	while(de[i] != NULL)
	{
		mfree(de[i++]);
	}
	mfree(de);
}

void DE_Sort1(DIR_ENTRY ***DE)
{
	DIR_ENTRY **de = *DE;
	unsigned int j = 0;
	unsigned int i = 0;
	while (de[i + 1] != NULL)
	{
		j = i + 1;
		while(de[j] != NULL)
		{
			if (strcmp(de[i]->file_name, de[j]->file_name) > 0)
			{
				DIR_ENTRY *tmp = de[i];
				de[i] = de[j];
				de[j] = tmp;
				continue;
			}
			j++;
		}
		i++;
	}
}

int GetExtByPath(char *dest, const char *path)
{
	char *ptr = strrchr(path, '.');
	if (ptr == NULL) return -1;
	ptr++;
	strcpy(dest, ptr);
	return 1;
}

int GetFNameWithoutExt(char *dest, const char *fname)
{
	char *ptr = strrchr(fname, '.');
	if (ptr == NULL) return -1;
	strncpy(dest, fname, ptr - fname);
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
