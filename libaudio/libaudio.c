#include <swilib.h>
#include "libaudio.h"
#include "../libsiemens/find_files.h"
#include "../libsiemens/strings.h"

int GetTagFromFile(A_TAG *tag, const char *path)
{
	FSTATS fs;
	unsigned int err;
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	
	FILE_PROP fp;
	zeromem(&fp, sizeof(FILE_PROP));
	int len;
	len = strlen(path);
	fp.filename = AllocWS(len);
	str_2ws(fp.filename, path, len);
	
	WSHDR *ws_fname, *ws_dir;
	char fname[128], dir[128];
	
	//имя файла
	GetFileNameByPath(fname, path);
	len = strlen(fname);
	ws_fname = AllocWS(len);
	str_2ws(ws_fname, fname, len);
	
	//директория
	GetDirByPath(dir, path);
	len = strlen(dir);
	ws_dir = AllocWS(len);
	str_2ws(ws_dir, dir, len);
	
	
	fp.type = 0x1800;
	fp.tag_title_ws  = tag->title;
	fp.tag_artist_ws = tag->artist;
	fp.tag_album_ws  = tag->album;
	fp.tag_track_ws  = tag->track;
	fp.unk_tag_2_ws  = tag->genre;
	
	
	GetFileProp(&fp, ws_fname, ws_dir);
	
	FreeWS(fp.filename);
	FreeWS(ws_fname);
	FreeWS(ws_dir);
	
	return 1;
}


#define M3U_HEADER  "#EXTM3U"
#define END_OF_LINE "\r\n"

int SaveM3U(DIR_ENTRY_LIST *top, const char *path)
{
	FSTATS fs;
	unsigned int err;
	
	if (!top) return -1;
	
	if (GetFileStats(path, &fs, &err) != -1) return -1;
	
	int fp = _open(path, A_WriteOnly + A_TXT + A_Append + A_Create, P_WRITE, &err);
	if (fp == -1) return -1;
	
	_write(fp, M3U_HEADER, 7, &err);
	_write(fp, END_OF_LINE, 2, &err);
	
	DIR_ENTRY_LIST *p = top;
	unsigned int len;
	while(p)
	{
		len = strlen(p->path);
		_write(fp, p->path, len, &err);
		_write(fp, END_OF_LINE, 2, &err);
		p = p->next;
	}
	_close(fp, &err);
}

int OpenM3U(DIR_ENTRY_LIST **list, const char *path)
{
	FSTATS fs;
	unsigned int err;
	
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	
	int fp = _open(path, A_ReadOnly + A_BIN, P_READ, &err);
	if (fp == -1) return -1;
	
	char *buffer = malloc(fs.size);
	_read(fp, buffer, fs.size, &err);
	buffer[fs.size] = '\0';

	DIR_ENTRY_LIST *prev;
	DIR_ENTRY_LIST *l  = malloc(sizeof(DIR_ENTRY_LIST));
	DIR_ENTRY_LIST *top = l;
	char file_path[256];
	unsigned int i = 0, j = 0;
	unsigned int total = 0;	
	
	while (i < fs.size)
	{
		if (buffer[i] != '\n' && buffer[i] != '\0')
		{
			if (buffer[i] == '#')
			{
				while(buffer[i] != '\n' && buffer[i] != '\0')
					i++;
			}
			else
			{
				if (buffer[i] != '\r')
					file_path[j++] = buffer[i];
			}
		}
		else
		{
			file_path[j] = '\0';
			j = 0;
			
			strcpy(l->path, file_path);
			A_TAG *tag = malloc(sizeof(A_TAG));
			tag->title  = AllocWS(128);
			tag->artist = AllocWS(128);
			tag->album  = AllocWS(128);
			tag->track  = AllocWS(128);
			tag->genre  = AllocWS(128);
			GetTagFromFile(tag, file_path);
			l->data = tag;
			
			
			
			prev = l;
			l->next = malloc(sizeof(DIR_ENTRY_LIST));
			l = l->next;
			l->prev = prev;
			
			
			total++;
		}
		i++;
	}
	_close(fp, &err);
	
	if (total)
	{
		top->prev = NULL;
		l = prev;
		mfree(l->next);
		l->next = NULL;
		*list = top;
	}
	else
	{
		mfree(l);
		return -1;
	}
	
	return total;
}