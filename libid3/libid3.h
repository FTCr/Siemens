#ifndef _LIBID3_H_
	#define _LIBID3_H_ 

#define ID3_ARRAY_SIZE 0x07

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
			WSHDR *year;
			WSHDR *comment;
			WSHDR *genre;
		};
		WSHDR *tag[ID3_ARRAY_SIZE];
	};
} ID3;

int GetID3(ID3 *id3, const char *path);

//clear id3 data
void FreeID3(ID3 *id3);

#endif
