#ifndef _LIBSIEMENS_FILES_H_
	#define _LIBSIEMENS_FILES_H_

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
			unsigned int uid6;
			unsigned int uid7;
			unsigned int uid8;
		};
		unsigned int data[8]; 
	}; 
} FIND_UIDS;

typedef struct
{
	void *next;
	void *prev;
	
	unsigned int file_size;
	short file_attr;
	unsigned int create_date_time;
	char dir[128];
	char fname[128];
} DIR_ENTRY_LIST;

int GetExtByPath(char *dest, const char *path);
unsigned int GetExtUid(const char *ext);
unsigned int GetExtUidByFileName(const char *path);
int GetFileNameWithoutExt(char *dest, const char *fname);
int GetDirByPath(char *dest, const char *path);
void utf8_2fname(char *dest, const char *source);

//обычный поиск файлов
unsigned int FindFiles(DIR_ENTRY_LIST **list, const char *dir, const char *mask);
//рекурсивный поиск файлов по uid(расширению)
unsigned int FindFilesRec(DIR_ENTRY_LIST **list, const char *dir, FIND_UIDS *fu);
//очистка
void FreeDEList(DIR_ENTRY_LIST **list);
void SortDEList(DIR_ENTRY_LIST **list);

#endif
