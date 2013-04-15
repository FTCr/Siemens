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

unsigned int FindFiles(DIR_ENTRY_LIST **list, const char *dir, const char *mask)
{
	DIR_ENTRY_LIST *top = NULL;
	unsigned int i = 0;
	
	char find[256];
	strcpy(find, dir);
	strcat(find, mask);
	
	DIR_ENTRY de;
	unsigned int err;
	LockSched();
	if (FindFirstFile(&de, find, &err))
	{
		DIR_ENTRY_LIST *prev, *de_list;
		top = malloc(sizeof(DIR_ENTRY_LIST));
		de_list = top;
		do
		{
			de_list->file_size        = de.file_size;
			de_list->file_attr        = de.file_attr;
			de_list->create_date_time = de.create_date_time;
			
			strcpy(de_list->dir,   de.folder_name);
			strcpy(de_list->fname, de.file_name);
			
			prev = de_list;
			de_list->next = malloc(sizeof(DIR_ENTRY_LIST));
			de_list = de_list->next;
			de_list->prev = prev;
			i++;
		}
		while (FindNextFile(&de, &err));
		top->prev = NULL;
		mfree(prev->next);
		prev->next = NULL;
	}
	UnlockSched();
	FindClose(&de, &err);
	*list = top;
	return i;
}

unsigned int FindFilesRec(DIR_ENTRY_LIST **list, const char *dir, FIND_UIDS *fu)
{
	DIR_ENTRY_LIST *top = NULL;
	int total = 0;
	
	char path[256];
	char find[256];
	char folder_name[128];
	
	strcpy(find, dir);
	strcat(find, "*");
	
	DIR_ENTRY de;
	unsigned int err;
	LockSched();
	if (FindFirstFile(&de, find, &err))
	{
		DIR_ENTRY_LIST *de_list, *prev;
		//самый первый запуск фукнции
		if (*list == NULL)
		{
			top = malloc(sizeof(DIR_ENTRY_LIST));
			de_list = top;
		}
		//еще где-то в рекурсии
		else
			de_list = *list;
		do
		{
			strcpy(path, de.folder_name);
			strcat(path, "\\");
			strcat(path, de.file_name);
			if (isdir(path, &err))
			{
				strcat(path, "\\");
				total += FindFilesRec(&de_list, path, fu);
			}
			else
			{
				if (fu)
				{
					unsigned int uid;
					for (int i = 0; i < 8; i++)
					{
						uid = GetExtUidByFileName(de.file_name);
						if (uid == fu->data[i]) goto COPY_DATA;
					}
				}
				else
				{
					COPY_DATA:
						de_list->file_size        = de.file_size;
						de_list->file_attr        = de.file_attr;
						de_list->create_date_time = de.create_date_time;
			
						strcpy(de_list->dir,   de.folder_name);
						strcpy(de_list->fname, de.file_name);
			
						prev = de_list;
						de_list->next = malloc(sizeof(DIR_ENTRY_LIST));
						de_list = de_list->next;
						de_list->prev = prev;
						total++;
				}
			}
		}
		while (FindNextFile(&de, &err));
		//самый первый запуск функции
		if (*list == NULL)
		{
			top->prev = NULL;
			mfree(prev->next);
			prev->next = NULL;
			*list = top;
		}
		//еще где-то в рекурсии
		else
		{
			*list = de_list;
		}
	}
	UnlockSched();
	FindClose(&de, &err);
	return total;
}

void FreeDEList(DIR_ENTRY_LIST **list)
{
	DIR_ENTRY_LIST *de_list = *list;
	while(de_list)
	{
		mfree(de_list);
		de_list = de_list->next;
	}
}

void SortDEList(DIR_ENTRY_LIST **list)
{
	DIR_ENTRY_LIST *de_list1, *de_list2;
	if (*list)
	{
		char *ptr1, *ptr2;
		
		de_list1 = *list;
		while (de_list1->next)
		{
			de_list2 = de_list1->next;
			while(de_list2)
			{
				//а вдруг сименовский юникод
				ptr1 = (de_list1->fname[0] == 0x1F) ? de_list1->fname + 1 : de_list1->fname;
				ptr2 = (de_list2->fname[0] == 0x1F) ? de_list2->fname + 1 : de_list2->fname;
				
				if (strcmp_nocase(ptr1, ptr2) > 0)
				{
					//копируем данные
					DIR_ENTRY_LIST tmp;
					memcpy(&tmp, de_list1, sizeof(DIR_ENTRY_LIST));
					memcpy(de_list1, de_list2, sizeof(DIR_ENTRY_LIST));
					memcpy(de_list2, &tmp, sizeof(DIR_ENTRY_LIST));
					//возвращаем расcтановку воиск
					void *ptr;
					ptr = de_list1->next;
					de_list1->next = de_list2->next;
					de_list2->next = ptr;
					continue;
				}
				de_list2 = de_list2->next;
			}
			de_list1 = de_list1->next;
		}
	}
}

/*void DE_Sort1_With_Dirs(DIR_ENTRY ***DE)
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
}*/
