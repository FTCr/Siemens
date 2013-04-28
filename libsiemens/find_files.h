#ifndef _LIBSIEMENS_FIND_FILES_H_
	#define _LIBSIEMENS_FIND_FILES_H_

#define MAX_UIDS 0x05

typedef struct
{
	union
	{
		struct
		{
			unsigned int uid1; 
			unsigned int uid2; 
			unsigned int uid3; 
			unsigned int uid4; 
			unsigned int uid5;
		};
		unsigned int data[MAX_UIDS]; 
	}; 
} FIND_UIDS;

typedef struct
{
	void *next;
	void *prev;
	
	char path[256];
	char dir[128];
	char fname[128];
	unsigned int file_size;
	short file_attr;
	unsigned int create_date_time;
	
	void *data;
} DIR_ENTRY_LIST;

//обычный поиск файлов
unsigned int FindFiles(DIR_ENTRY_LIST **list, const char *dir, const char *mask, void CallBack(DIR_ENTRY_LIST *ptr));
//рекурсивный поиск файлов по uid(расширению)
unsigned int FindFilesRec(DIR_ENTRY_LIST **list, const char *dir, FIND_UIDS *fu, void CallBack(DIR_ENTRY_LIST *ptr));
//получение элемента по id
DIR_ENTRY_LIST *GetDEListPtr(DIR_ENTRY_LIST *list, unsigned int n);
//удаление элемента
int CutDEListData(DIR_ENTRY_LIST **top, DIR_ENTRY_LIST **buffer, unsigned int n);
//вставка элемента из буфера
int PasteDEListData(DIR_ENTRY_LIST **top, DIR_ENTRY_LIST **buffer, unsigned int n);
//очистка
void FreeDEList(DIR_ENTRY_LIST *list, void CallBack(DIR_ENTRY_LIST *ptr));
//сортировка пузырьком
//void SortDEList(DIR_ENTRY_LIST **list);

#endif
