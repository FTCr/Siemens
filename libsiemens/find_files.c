#include <swilib.h>
#include "find_files.h"
#include "obs.h"
#include "strings.h"

unsigned int FindFiles(DIR_ENTRY_LIST **list, const char *dir, const char *mask, void CallBack(DIR_ENTRY_LIST *ptr))
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
			strcpy(de_list->path, de.folder_name);
			strcat(de_list->path, "\\");
			strcat(de_list->path, de.file_name);
			strcpy(de_list->dir,   de.folder_name);
			strcpy(de_list->fname, de.file_name);
			
			
			de_list->file_size        = de.file_size;
			de_list->file_attr        = de.file_attr;
			de_list->create_date_time = de.create_date_time;
			
			if (CallBack) CallBack(de_list);
			
			
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

unsigned int FindFilesRec(DIR_ENTRY_LIST **list, const char *dir, FIND_UIDS *fu, void CallBack(DIR_ENTRY_LIST *ptr))
{
	DIR_ENTRY_LIST *top = NULL;
	
	unsigned int total = 0;
	
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
		DIR_ENTRY_LIST *ptr = NULL;
		DIR_ENTRY_LIST *prev = NULL;
		if (!(*list))
		{
			top = malloc(sizeof(DIR_ENTRY_LIST));
			top->prev = NULL;
			ptr = top;
		}
		else
		{
			ptr = *list;
		}
		do
		{
			strcpy(path, de.folder_name);
			strcat(path, "\\");
			strcat(path, de.file_name);
			//isdir
			if (isdir(path, &err))
			{
				strcat(path, "\\");
				total += FindFilesRec(&ptr, path, fu, CallBack);
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
						strcpy(ptr->path, de.folder_name);
						strcat(ptr->path, "\\");
						strcat(ptr->path, de.file_name);
						strcpy(ptr->dir, de.folder_name);
						strcpy(ptr->fname, de.file_name);
						ptr->file_size        = de.file_size;
						ptr->file_attr        = de.file_attr;
						ptr->create_date_time = de.create_date_time;
						
						if (CallBack) CallBack(ptr);
						
						prev = ptr;
						ptr->next = malloc(sizeof(DIR_ENTRY_LIST));
						ptr = ptr->next;
						ptr->prev = prev;
						
						total++;
				}
			}
		}
		while (FindNextFile(&de, &err));
		if (!(*list))
		{
			*list = top;
			//delete last element
			if (prev)
			{
				mfree(prev->next);
				prev->next = NULL;
			}
		}
		else
			*list = ptr;
	}
	UnlockSched();
	FindClose(&de, &err);
	return total;
}

DIR_ENTRY_LIST *GetDEListPtr(DIR_ENTRY_LIST *top, unsigned int n)
{
	DIR_ENTRY_LIST *ptr = top;
	unsigned int i = 0;
	while(i < n)
	{	
		if (!ptr->next) return NULL;
		ptr = ptr->next;
		i++;
	}
	return ptr;
}

unsigned int GetDEListTotalItems(DIR_ENTRY_LIST *top)
{
	unsigned int i = 0;
	DIR_ENTRY_LIST *p = top;
	while (p)
	{
		i++;
		p = p->next;
	}
	return i;
}

int CutDEListData(DIR_ENTRY_LIST **top, DIR_ENTRY_LIST **buffer, unsigned int n)
{
	DIR_ENTRY_LIST *ptr = GetDEListPtr(*top, n);
	
	if (!ptr) return -1;
	
	if (!(*buffer)) *buffer = malloc(sizeof(DIR_ENTRY_LIST));
	memcpy(*buffer, ptr, sizeof(DIR_ENTRY_LIST));
	
	DIR_ENTRY_LIST *p1, *p2;
	if ((*top)->next)
	{
		if (!n)
		{
			*top = (*top)->next;
			(*top)->prev = NULL;
		}
		else
		{
			p1 = ptr->prev;
			if (ptr->next)
			{
				p2 = ptr->next;
				p2->prev = p1;
				p1->next = ptr->next;
			}
			else
			{
				p1->next = NULL;
			}
		}
		mfree(ptr);
	}
	else
	{
		mfree(*top);
		*top = NULL;
	}
	return 1;
}

int PasteDEListData(DIR_ENTRY_LIST **top, DIR_ENTRY_LIST **buffer, unsigned int n)
{
	DIR_ENTRY_LIST *paste;
	void func(void)
	{
		paste = malloc(sizeof(DIR_ENTRY_LIST));
		memcpy(paste, *buffer, sizeof(DIR_ENTRY_LIST));
		mfree((*buffer));
		*buffer = NULL;
	}
	if (*top)
	{
		DIR_ENTRY_LIST *ptr = GetDEListPtr(*top, n);
	
		if (!ptr) return -1;
		
		if (!(*buffer)) return -1;
		
		func();
		
		DIR_ENTRY_LIST *p1, *p2;
		if (!n)
		{
			p1 = *top;
			*top = paste;
			(*top)->prev = NULL;
			(*top)->next = p1;
			p1->prev = paste;
		}
		else
		{
			p1 = ptr->prev;
			p2 = p1->next;
		
			p1->next = paste;
			paste->next = p2;
			paste->prev = p1;
			p2->prev = paste;
		}
	}
	else
	{
		func();
		paste->prev = NULL;
		paste->next = NULL;
		*top = paste;
	}
	return 1;
}

void FreeDEList(DIR_ENTRY_LIST *top, void CallBack(DIR_ENTRY_LIST *ptr))
{
	DIR_ENTRY_LIST *next;
	while(top)
	{
		next = top->next;
		if (CallBack) CallBack(top);
		mfree(top);
		top = next;
	}
}

/*void SortDEList(DIR_ENTRY_LIST **list) //кривоватенько что-то, а что не помню
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
}*/

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
