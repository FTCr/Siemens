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


int GetExtByPath(char *dest, const char *path);
unsigned int GetExtUid(const char *ext);
unsigned int GetExtUidByFileName(const char *path);
int GetFNameWithoutExt(char *dest, const char *fname);
int GetDirByPath(char *dest, const char *path);
void utf8_2fname(char *dest, const char *source);
//обычный поиск файлов
unsigned int FindFiles(DIR_ENTRY ***DE, const char *dir, const char *mask);
//рекурсивный поиск файлов по uid
unsigned int FindFilesRec(DIR_ENTRY ***DE, const char *dir, FIND_UIDS *fu);
//очистка
void DE_Free(DIR_ENTRY ***DE);
//сортировка пузырьком
void DE_Sort1(DIR_ENTRY ***DE);
//сортировка пузырьком, но папки наверху
void DE_Sort1_With_Dirs(DIR_ENTRY ***DE);

#endif
