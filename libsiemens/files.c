#include <swilib.h>
#include "files.h"
#include "strings.h"

unsigned int GetExtUid(const char *ext)
{
	unsigned int uid = 0;
	
	unsigned int len = strlen(ext);
	
	if (!len) return uid;
	
	
	
	if (strcmp_nocase(ext, "mp3") == 0)
		uid = UID_MP3;
	else if (strcmp_nocase(ext, "m3u") == 0)
		uid = UID_M3U;
	else if (strcmp_nocase(ext, "jar") == 0)
		uid = UID_JAR;
	else if (strcmp_nocase(ext, "jad") == 0)
		uid = UID_JAD;
	else if (strcmp_nocase(ext, "mid") == 0)
		uid = UID_MID;
	else if (strcmp_nocase(ext, "amr") == 0)
		uid = UID_AMR;
	else if (strcmp_nocase(ext, "imy") == 0)
		uid = UID_IMY;
	else if (strcmp_nocase(ext, "srt") == 0)
		uid = UID_SRT;
	else if (strcmp_nocase(ext, "aac") == 0)
		uid = UID_AAC;
	else if (strcmp_nocase(ext, "wav") == 0)
		uid = UID_WAV;
	else if (strcmp_nocase(ext, "jts") == 0)
		uid = UID_JTS;
	else if (strcmp_nocase(ext, "xmf") == 0)
		uid = UID_XMF;
	else if (strcmp_nocase(ext, "m4a") == 0)
		uid = UID_M4A;
	else if (strcmp_nocase(ext, "bmx") == 0)
		uid = UID_BMX;
	else if (strcmp_nocase(ext, "wbmp") == 0)
		uid = UID_WBMP;
	else if (strcmp_nocase(ext, "bmp") == 0)
		uid = UID_BMP;
	else if (strcmp_nocase(ext, "jpg") == 0)
		uid = UID_JPG;
	else if (strcmp_nocase(ext, "jpeg") == 0)
		uid = UID_JPG;
	else if (strcmp_nocase(ext, "png") == 0)
		uid = UID_PNG;
	else if (strcmp_nocase(ext, "gif") == 0)
		uid = UID_GIF;
	else if (strcmp_nocase(ext, "svg") == 0)
		uid = UID_SVG;
	else if (strcmp_nocase(ext, "3gp") == 0)
		uid = UID_3GP;
	else if (strcmp_nocase(ext, "mp4") == 0)
		uid = UID_M4A;
	else if (strcmp_nocase(ext, "sdp") == 0)
		uid = UID_SDP;
	else if (strcmp_nocase(ext, "pvx") == 0)
		uid = UID_PVX;
	else if (strcmp_nocase(ext, "sdt") == 0)
		uid = UID_SDT;
	else if (strcmp_nocase(ext, "ldb") == 0)
		uid = UID_LDB;
	else if (strcmp_nocase(ext, "txt") == 0)
		uid = UID_TXT;
	else if (strcmp_nocase(ext, "url") == 0)
		uid = UID_URL;
	return uid;
}

unsigned int GetExtUidByFileName(const char *path)
{
	char *ptr = strrchr(path, '.');
	
	if (!ptr) return 0;
	
	ptr++;
	
	return GetExtUid(ptr);
}

int GetExtByPath(char *dest, const char *path)
{
	char *ptr = strrchr(path, '.');
	if (ptr == NULL) return -1;
	ptr++;
	strcpy(dest, ptr);
	return 1;
}

int GetFileNameWithoutExt(char *dest, const char *fname)
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
	strncpy(dest, path, ptr - path);
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
			memcpy(de[i], &tmp_de, sizeof(DIR_ENTRY));
			de[i + 1] = NULL;
			i++;
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
			//а вдруг сименовский юникод?
			char *ptr1, *ptr2;
			ptr1 = (de[i]->file_name[0] == 0x1F) ? de[i]->file_name + 1 : de[i]->file_name;
			ptr2 = (de[j]->file_name[0] == 0x1F) ? de[j]->file_name + 1 : de[j]->file_name;
			
			if (strcmp_nocase(ptr1, ptr2) > 0)
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
