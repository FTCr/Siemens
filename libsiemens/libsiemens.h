#ifndef _LIBSIEMENS_H_
	#define _LIBSIEMENS_H_

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
void DE_Sort1(DIR_ENTRY ***DE);  //сортировка пузырьком
void DE_Sort1_With_Dirs(DIR_ENTRY ***DE); //сортировка пузырьком, но папки наверху
int GetExtByPath(char *dest, const char *path);
int GetFNameWithoutExt(char *dest, const char *fname);
void utf8_2fname(char *dest, const char *source);


IMGHDR *CreateIMGHDRFromImgFile(const char *path);
void DrawIMGHDR(IMGHDR *img, int x, int y, int x2, int y2, int bleed_x, int bleed_y);
void FreeIMGHDR(IMGHDR *img);
unsigned int GetWidthWS(WSHDR *ws, const int font);

void IPC_SendMessage(IPC_REQ *ipc, const char *name_to, const char *name_from, const int mess, void *data);


int InitLang(const char *path, char ***lgp);
void FreeLang(char ***lgp);

unsigned int GetExtUidByFileName(const char *path);

HObj CreateHObjFromImgFile(const char *path);
IMGHDR *HObj2IMGHDR(HObj obj);
void SetScaling(HObj obj, short *w, short *h, short width, short height);
HObj CreateHObjFromAudioFile(char *path);
unsigned int Obs_SoundGetVolume(HObj obj);

void ExecEntrypoint(const char *name);
void ExecShortcut(const char *name);
void ExecFile(const char *path);
unsigned int GetCSMIDFromAddr(const char *addr);

int str2lower(char *dest, const char *src); //преобразование строки в нижний регистр
int strcmp_nocase(const char *s, const char *d); //аналог strcmp, но без чувствительности к регистру

int *SetSWIHook(int swi_num, void *proc);
void DestroySWIHook(int swi_num, unsigned int *addr);

int GetStrDataFromSystemConf(char *dest, const char *path, const char *keyword);
#endif
