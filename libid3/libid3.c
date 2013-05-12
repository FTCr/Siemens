#include <swilib.h>
#include "libid3.h"

#define ID3_HEAD_LEN 0x03

#define ID3_V1_HEAD  "ID3"

#define ID3_V2_HDR        "TAG"
#define ID3_V2_HDR_SIZE    0x0A
#define ID3_V2_FRAME_COUNT  0x07
#define ID3_V2_4_FRAME_LEN  0x04

typedef struct
{
	char head[3];
	char ver;
	char rev;
	char flags;
	unsigned int size;
} ID3_V2_HEADER;

//enum frame_id {title, artist, album, track, year, comment, genre};

const char *id3_v2_4_frame[ID3_V2_FRAME_COUNT] =
{
	"TIT2", //title
	"TPE1", //artist
	"TALB", //album
	"TRCK", //track
	"TDRC", //year
	"COMM", //comment
	"TCON", //genre
};

int ID3_V2_Helper(const char *buffer)
{
	int i = 0;
	while(i < ID3_V2_FRAME_COUNT)
	{
		if (strncmp(buffer, id3_v2_4_frame[i], ID3_V2_4_FRAME_LEN) == 0)
			return i;
		i++;
	}
	return -1;
}

//not inline! :-(
int ID3_V2_4_BitCock(const char *p)
{
	return (p[3] & 0xFF) | ((p[2] & 0xFF) << 7 ) | ((p[1] & 0xFF) << 14 ) | ((p[0] & 0xFF) << 21 );
}

int GetID3(ID3 *id3, const char *path)
{
	FSTATS fs;
	unsigned int err;
	
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	
	int fp = _open(path, A_ReadOnly + A_BIN, P_READ, &err);
	if (fp == -1) return -1;
	
	char head[ID3_HEAD_LEN];
	if (_read(fp, head, ID3_HEAD_LEN, &err) < ID3_HEAD_LEN) //empty?
	{
		_close(fp, &err);
		return -1;
	}
	
	//get tag version
	enum id3_ver_enum {id3_v1, id3_v2};
	int id3_ver;
	if (strncmp(head, ID3_V1_HEAD, ID3_HEAD_LEN) != 0)
	{
		id3_ver = id3_v1;
		_close(fp, &err);
		return -1;
	}
	else if (strncmp(head, ID3_V2_HDR, ID3_HEAD_LEN) != 0)
	{
		id3_ver = id3_v2;
	}
	else
	{
		_close(fp, &err);
		return -1;
	}
	//read id3v2 header
	if (id3_ver == id3_v2)
	{
		ID3_V2_HEADER header;
		strncpy(header.head, head, ID3_HEAD_LEN);
		header.head[ID3_HEAD_LEN] = '\0';
		
		char *buffer = malloc(ID3_V2_HDR_SIZE);
		_read(fp, buffer, ID3_V2_HDR_SIZE - ID3_HEAD_LEN, &err);
		header.ver   = buffer[0];
		header.rev   = buffer[1];
		header.flags = buffer[2];
		header.size  = ID3_V2_4_BitCock(buffer + 3);
		//header.size = (buffer[6] & 0xFF) | ((buffer[5] & 0xFF) << 7 ) | ((buffer[4] & 0xFF) << 14 ) | ((buffer[3] & 0xFF) << 21 );
		
		//only v2.4
		unsigned int frame_lenght;
		if (header.ver != 0x04)
		{
			mfree(buffer);
			_close(fp, &err);
			return 0;
		}
		frame_lenght = 0x0A;
		
		mfree(buffer);
		buffer = malloc(header.size + 5);
		_read(fp, buffer, header.size + 4, &err);
		zeromem(id3, sizeof(ID3));
		
		unsigned int tag_lenght;
		unsigned int frame_id = 0;
		int helper;
		unsigned int i = 0;
		
		while (i < header.size && frame_id < ID3_V2_FRAME_COUNT)
		{
			if (buffer[i] >= 'A' && buffer[i] <= 'Z')
			{
				helper = ID3_V2_Helper(buffer + i);
				if (helper != -1)
				{
					if (helper == 5) //wtf?
					{
						tag_lenght = ID3_V2_4_BitCock(buffer + i + ID3_V2_4_FRAME_LEN) - 5;
						i += frame_lenght + 5;
					}
					else
					{
						tag_lenght = ID3_V2_4_BitCock(buffer + i + ID3_V2_4_FRAME_LEN) - 1;
						i += frame_lenght + 1;
					}
					id3->tag[helper] = AllocWS(tag_lenght + 1);
					utf8_2ws(id3->tag[helper], buffer + i, tag_lenght);
					frame_id++;
					i += tag_lenght - 1;
				}
			}
			i++;
		}
		mfree(buffer);
	}
	_close(fp, &err);
	return 1;
}

void FreeID3(ID3 *id3)
{
	if (id3->title)
		FreeWS(id3->title);
	if (id3->album)
		FreeWS(id3->album);
	if (id3->artist)
		FreeWS(id3->artist);
	if (id3->year)
		FreeWS(id3->year);
	if (id3->comment)
		FreeWS(id3->comment);
	if (id3->track)
		FreeWS(id3->track);
	if (id3->genre)
		FreeWS(id3->genre);
}