#include <swilib.h>
#include "files.h"
#include "string_utils.h"
#include "obs.h"

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
	}
	FindClose(&tmp_de, &err);
	*DE = de;
	return i;
}

unsigned int FindFilesRec(DIR_ENTRY ***DE, const char *dir, FIND_UIDS *fu)
{
	DIR_ENTRY tmp_de;
	static int total = 0;
	
	unsigned int err;
	char path[128], find[128], folder_name[128];
	sprintf(find, "%s%s", dir, "*");
	LockSched();
	if (FindFirstFile(&tmp_de, find, &err))
	{
		do
		{
			sprintf(path, "%s\\%s", tmp_de.folder_name, tmp_de.file_name);
			if (isdir(path, &err))
			{
				strcat(path, "\\");
				FindFilesRec(DE, path, fu);
			}
			else
			{
				if (fu)
				{
					unsigned int uid;
					for (int i = 0; i < 8; i++)
					{
						if (fu->data[i])
						{
							uid = GetExtUidByFileName(tmp_de.file_name);
							if (uid == fu->data[i]) goto SAVE;
						}
					}
				}
				else
				{
					SAVE:
						(*DE) = realloc((*DE), sizeof(DIR_ENTRY*) * (total + 2));
						(*DE)[total] = malloc(sizeof(DIR_ENTRY));
						memcpy((*DE)[total], &tmp_de, sizeof(DIR_ENTRY));
						(*DE)[(total + 1)] = NULL;
						total++;
				}
			}
		} while(FindNextFile(&tmp_de, &err));
	}
	FindClose(&tmp_de, &err);
	UnlockSched();
	return total;
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

void DE_Sort1_With_Dirs(DIR_ENTRY ***DE)
{
	DIR_ENTRY **de = *DE;
	DIR_ENTRY **de_dirs  = NULL;
	DIR_ENTRY **de_files = NULL;
	unsigned int err;
	unsigned int i = 0;
	unsigned int d_id = 0;
	unsigned int f_id = 0;
	//раскидываем файлы и директории по отдельным переменным
	char path[128];
	while (de[i] != NULL)
	{
		sprintf(path, "%s\\%s", de[i]->folder_name, de[i]->file_name);
		if (isdir(path, &err))
		{
			de_dirs = realloc(de_dirs, sizeof(DIR_ENTRY*) * (d_id + 2));
			de_dirs[d_id]     = de[i];
			de_dirs[d_id + 1] = NULL;
			d_id++;
		}
		else
		{
			de_files = realloc(de_files, sizeof(DIR_ENTRY*) * (f_id + 2));
			de_files[f_id]     = de[i];
			de_files[f_id + 1] = NULL;
			f_id++;
		}
		i++;
	}
	
	if (de_dirs)
	{
		//сортируем каталоги
		DE_Sort1(&de_dirs);
		//пихаем их на выход
		i = 0;
		while (de_dirs[i] != NULL)
			de[i] = de_dirs[i++];
		//чистим
		mfree(de_dirs);
	}
	if (de_files)
	{
		//сортируем файлы
		DE_Sort1(&de_files);
		//пихаем их на выход
		f_id = i;
		i = 0;
		while (de_files[i] != NULL)
		{
			de[f_id] = de_files[i];
			f_id++;
			i++;
		}
		//чистота залог успеха
		mfree(de_files);
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
