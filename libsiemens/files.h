#ifndef _LIBS_FILES_H_
	#define _LIBS_FILES_H_

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

unsigned int FindFiles(DIR_ENTRY ***DE, const char *dir, const char *mask); //простой поиск файлов
unsigned int FindFilesRec(DIR_ENTRY ***DE, const char *dir, FIND_UIDS *fu); // рекурсивный поиск файлов по его uid

void DE_Free(DIR_ENTRY ***DE);
//-------сортировка----------//
void DE_Sort1(DIR_ENTRY ***DE);  //пузырьком
void DE_Sort1_With_Dirs(DIR_ENTRY ***DE); //пузырьком, но папки наверху

int GetExtByPath(char *dest, const char *path);
int GetFNameWithoutExt(char *dest, const char *fname);
void utf8_2fname(char *dest, const char *source);

#endif
