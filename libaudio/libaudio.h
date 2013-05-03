#ifndef _LIBAUDIO_H_
	#define _LIBAUDIO_H_ 

//в будущем нужно добавить редактор тэгов
	
#include "../libsiemens/find_files.h"

#define A_TAG_MAX 0x05

typedef struct
{
	union
	{
		struct
		{
			WSHDR *title;
			WSHDR *artist;
			WSHDR *album;
			WSHDR *track;
			WSHDR *genre;
		};
		WSHDR *data[A_TAG_MAX];
	};
} A_TAG;

int GetTagFromFile(A_TAG *info, const char *path);

int SaveM3U(DIR_ENTRY_LIST *top, const char *path);
int OpenM3U(DIR_ENTRY_LIST **list, const char *path);

#endif
