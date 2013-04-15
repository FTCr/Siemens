#include <swilib.h>
#include "find_files.h"
#include "obs.h"
#include "strings.h"

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
					for (int i = 0; i < MAX_UIDS; i++)
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
