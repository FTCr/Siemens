#include <swilib.h>
#include "libsiemens.h"

unsigned int FindFiles(DIR_ENTRY ***DE, const char *dir, const char *mask)
{
	DIR_ENTRY tmp_de;
	DIR_ENTRY **de = NULL;
	char find[128];
	unsigned int err;
	unsigned int i = 0;
	sprintf(find, "%s%s", dir, mask);
	
	if (FindFirstFile(&tmp_de, find, &err))
	{
		do
		{
			de = realloc(de, sizeof(DIR_ENTRY*) * (i + 2));
			de[i] = malloc(sizeof(DIR_ENTRY));
			memcpy(de[i], &tmp_de, sizeof(DIR_ENTRY));
			de[i + 1] = NULL;
			i++;
		} while(FindNextFile(&tmp_de, &err));
	}
	FindClose(&tmp_de, &err);
	*DE = de;
	return i;
}

unsigned int FindFilesRec(DIR_ENTRY ***DE, const char *dir, FIND_UIDS *fu)
{
	DIR_ENTRY tmp_de;
	static int total = 0;
	
	unsigned int err;
	char path[128], find[128], folder_name[128];
	sprintf(find, "%s%s", dir, "*");
	LockSched();
	if (FindFirstFile(&tmp_de, find, &err))
	{
		do
		{
			sprintf(path, "%s\\%s", tmp_de.folder_name, tmp_de.file_name);
			if (isdir(path, &err))
			{
				strcat(path, "\\");
				FindFilesRec(DE, path, fu);
			}
			else
			{
				if (fu)
				{
					unsigned int uid;
					for (int i = 0; i < 8; i++)
					{
						if (fu->data[i])
						{
							uid = GetExtUidByFileName(tmp_de.file_name);
							if (uid == fu->data[i]) goto SAVE;
						}
					}
				}
				else
				{
					SAVE:
						(*DE) = realloc((*DE), sizeof(DIR_ENTRY*) * (total + 2));
						(*DE)[total] = malloc(sizeof(DIR_ENTRY));
						memcpy((*DE)[total], &tmp_de, sizeof(DIR_ENTRY));
						(*DE)[(total + 1)] = NULL;
						total++;
				}
			}
		} while(FindNextFile(&tmp_de, &err));
	}
	FindClose(&tmp_de, &err);
	UnlockSched();
	return total;
}


void DE_Free(DIR_ENTRY ***DE)
{
	DIR_ENTRY **de = *DE;
	unsigned int i = 0;
	while(de[i] != NULL)
	{
		mfree(de[i++]);
	}
	mfree(de);
}

void DE_Sort1(DIR_ENTRY ***DE)
{
	DIR_ENTRY **de = *DE;
	unsigned int j = 0;
	unsigned int i = 0;
	while (de[i + 1] != NULL)
	{
		j = i + 1;
		while(de[j] != NULL)
		{
			if (strcmp_nocase(de[i]->file_name, de[j]->file_name) > 0)
			{
				DIR_ENTRY *tmp = de[i];
				de[i] = de[j];
				de[j] = tmp;
				continue;
			}
			j++;
		}
		i++;
	}
}

void DE_Sort1_With_Dirs(DIR_ENTRY ***DE)
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
}

int GetExtByPath(char *dest, const char *path)
{
	char *ptr = strrchr(path, '.');
	if (ptr == NULL) return -1;
	ptr++;
	strcpy(dest, ptr);
	return 1;
}

int GetFNameWithoutExt(char *dest, const char *fname)
{
	char *ptr = strrchr(fname, '.');
	if (ptr == NULL) return -1;
	strncpy(dest, fname, ptr - fname);
	return 1;
}

void utf8_2fname(char *dest, const char *source)
{
	int len = strlen(source) + 16;
	WSHDR *ws = AllocWS(len);
	utf8_2ws(ws, source, len);
	ws_2str(ws, dest, len);
	FreeWS(ws);
}



IMGHDR *CreateIMGHDRFromImgFile(const char *path)
{
	IMGHDR *img;
	HObj obj;
	short w, h;
	
	obj = CreateHObjFromImgFile(path);
	
	//для jpg обязательно нужны размеры, иначе пикает
	Obs_GetInputImageSize (obj, &w, &h);
	Obs_SetOutputImageSize (obj, w, h);
	
	Obs_Start(obj);
	
	
	img = HObj2IMGHDR(obj);
	Obs_DestroyObject(obj);
	
	return img;
}

void DrawIMGHDR(IMGHDR *img, int x, int y, int x2, int y2, int bleed_x, int bleed_y)
{
	RECT rc;
	DRWOBJ drwobj;
	if (!bleed_x && !bleed_y)
	{
		StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
		SetPropTo_Obj5(&drwobj, &rc, 0, img);
	}
	else
	{
		StoreXYXYtoRECT(&rc, x, y, x2, y2);
		SetProp2ImageOrCanvas(&drwobj, &rc, 0, img, bleed_x, bleed_y);
	}
	SetColor(&drwobj, NULL, NULL);
	DrawObject(&drwobj);
}

void FreeIMGHDR(IMGHDR *img)
{
	if (img)
	{
		if (img->bitmap)
			mfree(img->bitmap);
		mfree(img);
	}
}

unsigned int GetWidthWS(WSHDR *ws, const int font)
{
	return (Get_WS_width(ws, font) + 1);
}



void IPC_SendMessage(IPC_REQ *ipc, const char *name_to, const char *name_from, const int mess, void *data)
{
	ipc->name_to   = name_to;
	ipc->name_from = name_from;
	ipc->data      = data;
	GBS_SendMessage(MMI_CEPID, MSG_IPC, mess, ipc);
}


int InitLang(const char *path, char ***lgp)
{
	FSTATS fs;
	unsigned int err;

	if (GetFileStats(path, &fs, &err) == -1) return -1;

	int fp = _open(path, A_ReadOnly + A_BIN, P_READ, &err);
	if (fp == -1) return -1;

	char *buffer = malloc(fs.size + 1);
	buffer[fs.size] = '\0';
	_read(fp, buffer, fs.size, &err);
	char tmp[128];
	unsigned int pos = 0, len = 0, total = 0;

	char **tmp_lgp = NULL;
	while (pos <= fs.size)
	{
		if (buffer[pos] == '\n' || buffer[pos] == '\r' || buffer[pos] == '\0')
		{
			if (len)
			{
				tmp_lgp = realloc(tmp_lgp, sizeof(char*) * (total + 2));
				tmp_lgp[total]     = malloc(len + 1);
				tmp_lgp[total + 1] = NULL;

				memcpy(tmp_lgp[total], tmp, len);
				*((char*)(tmp_lgp[total++]) + len) = '\0';

				len = 0;
				*lgp=tmp_lgp;
			}
		}
		else
			tmp[len++] = buffer[pos];
				pos++;
	}
	mfree(buffer);
	_close(fp, &err);
	return total;
}

void FreeLang(char ***lgp)
{
	char **ptr = *lgp;
	if (ptr)
	{
		for (unsigned int i = 0; ptr[i] != NULL; i++)
			mfree(ptr[i]);
		mfree(ptr);
	}
}


unsigned int GetExtUid(const char *ext)
{
	unsigned int uid = 0;
	
	unsigned int len = strlen(ext);
	
	if (!len) return uid;
	
	
	
	if (strcmp_nocase(ext, "mp3") == 0)
		uid = UID_MP3;
	else if (strcmp_nocase(ext, "m3u") == 0)
		uid = UID_M3U;
	else if (strcmp_nocase(ext, "jar") == 0)
		uid = UID_JAR;
	else if (strcmp_nocase(ext, "jad") == 0)
		uid = UID_JAD;
	else if (strcmp_nocase(ext, "mid") == 0)
		uid = UID_MID;
	else if (strcmp_nocase(ext, "amr") == 0)
		uid = UID_AMR;
	else if (strcmp_nocase(ext, "imy") == 0)
		uid = UID_IMY;
	else if (strcmp_nocase(ext, "srt") == 0)
		uid = UID_SRT;
	else if (strcmp_nocase(ext, "aac") == 0)
		uid = UID_AAC;
	else if (strcmp_nocase(ext, "wav") == 0)
		uid = UID_WAV;
	else if (strcmp_nocase(ext, "jts") == 0)
		uid = UID_JTS;
	else if (strcmp_nocase(ext, "xmf") == 0)
		uid = UID_XMF;
	else if (strcmp_nocase(ext, "m4a") == 0)
		uid = UID_M4A;
	else if (strcmp_nocase(ext, "bmx") == 0)
		uid = UID_BMX;
	else if (strcmp_nocase(ext, "wbmp") == 0)
		uid = UID_WBMP;
	else if (strcmp_nocase(ext, "bmp") == 0)
		uid = UID_BMP;
	else if (strcmp_nocase(ext, "jpg") == 0)
		uid = UID_JPG;
	else if (strcmp_nocase(ext, "jpeg") == 0)
		uid = UID_JPG;
	else if (strcmp_nocase(ext, "png") == 0)
		uid = UID_PNG;
	else if (strcmp_nocase(ext, "gif") == 0)
		uid = UID_GIF;
	else if (strcmp_nocase(ext, "svg") == 0)
		uid = UID_SVG;
	else if (strcmp_nocase(ext, "3gp") == 0)
		uid = UID_3GP;
	else if (strcmp_nocase(ext, "mp4") == 0)
		uid = UID_M4A;
	else if (strcmp_nocase(ext, "sdp") == 0)
		uid = UID_SDP;
	else if (strcmp_nocase(ext, "pvx") == 0)
		uid = UID_PVX;
	else if (strcmp_nocase(ext, "sdt") == 0)
		uid = UID_SDT;
	else if (strcmp_nocase(ext, "ldb") == 0)
		uid = UID_LDB;
	else if (strcmp_nocase(ext, "txt") == 0)
		uid = UID_TXT;
	else if (strcmp_nocase(ext, "url") == 0)
		uid = UID_URL;
	return uid;
}

unsigned int GetExtUidByFileName(const char *path)
{
	char *ptr = strrchr(path, '.');
	
	if (!ptr) return 0;
	
	ptr++;
	
	return GetExtUid(ptr);
}

HObj CreateHObjFromImgFile(const char *path)
{
	HObj obj;
	unsigned int err = 0;
	unsigned int uid = GetExtUidByFileName(path);
	if (uid >= UID_BMX && uid <= UID_SVG)
	{
		#ifdef NEWSGOLD
			obj = Obs_CreateObject(uid, 0x2D, 0, 0x80A8, 1, 1, &err);
		#else
			obj = Obs_CreateObject(uid, 0x29, 2, 0x1C7, 1, 1, &err);
		#endif
		if (err) return -1;
		
		
		unsigned int len = strlen(path);
		WSHDR *ws = AllocWS(len);
		str_2ws (ws, path, len);
		Obs_SetInput_File(obj, 0, ws);
		FreeWS(ws);
		Obs_GetInfo(obj, 0);
		return obj;
	}
	return -1;
}

IMGHDR *HObj2IMGHDR(HObj obj)
{
	IMGHDR *img = malloc(sizeof(IMGHDR));
	IMGHDR *img_tmp;
	Obs_Output_GetPictstruct(obj, &img_tmp);
	unsigned int bs = CalcBitmapSize (img_tmp->w, img_tmp->h, img_tmp->bpnum);
	img->w      = img_tmp->w;
	img->h      = img_tmp->h;
	img->bpnum  = img_tmp->bpnum;
	img->bitmap = malloc(bs);
	memcpy(img->bitmap, img_tmp->bitmap, bs);
	return img;
}

///////////////////////////////////////////////////////
//*******************работа с аудио******************//
///////////////////////////////////////////////////////


HObj CreateHObjFromAudioFile(char *path)
{
	HObj obj;
	unsigned uid = GetExtUidByFileName(path); 
	unsigned int err = 0;
	obj = Obs_CreateObject(uid, 0x34, 2, 0, 1, 0, &err);
	if (!err)
	{
		int len = strlen(path);
		WSHDR *ws = AllocWS(len);
		str_2ws (ws, path, len);
		Obs_SetInput_File(obj, 0, ws);
		#ifdef ELKA  
			Obs_Mam_SetPurpose(obj, 0x16); 
		#else
			Obs_Sound_SetPurpose(obj, 0x16);
		#endif
		Obs_Prepare(obj);
		return obj;
	}
	return 0;
};

unsigned int Obs_SoundGetVolume(HObj obj)
{
	char vol = 0;
	Obs_Sound_GetVolume(obj, &vol);
	return vol;
}

void ExecEntrypoint(const char *name)
{
	typedef void (*voidfunc)(); 
	unsigned int addr = strtoul(name, 0, 16);
	voidfunc pp = (voidfunc)addr;
	SUBPROC((void*)pp);
}

void ExecShortcut(const char *name)
{
	unsigned int* addr = (unsigned int*)GetFunctionPointer((char*)name);
	if (addr)
	{
		typedef void (*voidfunc)(); 
		#ifdef NEWSGOLD          
			voidfunc pp = (voidfunc)*(addr + 4);
		#else
			voidfunc pp = (voidfunc)addr; 
		#endif 
		SUBPROC((void*)pp);
	}
}

void ExecFile(const char *path)
{
	unsigned int len = strlen(path);
	WSHDR *ws = AllocWS(len + 1);
	str_2ws(ws, path, len);
	ExecuteFile(ws, NULL, NULL);
	FreeWS(ws);
}

unsigned int GetCSMIDFromAddr(const char *addr)
{
	CSM_RAM *p;
	p = CSM_root()->csm_q->csm.first;
	while (p)
	{
		void *player_csm_addr = NULL;
		player_csm_addr = (void*)strtoul(addr, NULL, 16);
		if (p->constr == player_csm_addr) break;
		p = p->next;
	}
	if (p) return p->id;
	return 0;
}


int str2lower(char *dest, const char *src)
{
	unsigned int len = strlen(src);
	if (!len) return -1;
	
	char sym;
	unsigned int i = 0;
	while(i != len)
	{
		sym = src[i];
		if (src[i] >= 'A' && src[i] <= 'Z')
		{
			dest[i] = src[i] + ('a' - 'A');
		}
		else if (src[i] >= 0x90 && src[i] <= 0xAF)
		{
			dest[i] = src[i] + (0xB0 - 0x90);
		}
		else
		{
			dest[i] = src[i];
		}
		i++;
	}
	dest[len] = '\0';
	return i;
}

int strcmp_nocase(const char *s, const char *d)
{
	int cs;
	int ds;
	do
	{
		cs = *s++;
		if (cs&0x40) cs &= 0xDF;
		ds = *d++;
		if (ds & 0x40) ds &= 0xDF;
		cs -= ds;
		if (cs) break;
	}
	while(ds);
	return(cs);
}

int *SetSWIHook(int swi_num, void *proc) 
{ 
	LockSched(); 
	unsigned int *addr = NULL;
	unsigned int *lib = (int *)(*((int *)(int*)LIB_TOP())); 
	if (lib == NULL) return NULL;
	addr = (unsigned int*)lib[swi_num];
	lib[swi_num] = (unsigned int)proc; 
	UnlockSched(); 
	return addr;
}

void DestroySWIHook(int swi_num, unsigned int *addr)
{
	LockSched();
	unsigned int *lib = (int *)(*((int *)(int*)LIB_TOP())); 
	if (lib == NULL) return;
	lib[swi_num] = (unsigned int)addr;
	UnlockSched();
}

int Parse(char *dest, const char *buffer, unsigned int size, const char *keyword)
{
	int ret = -1;
	unsigned int i = 0;
	unsigned int j = 1;
	unsigned int len = strlen(keyword);
	while (i != size)
	{
		if (buffer[i] == keyword[0]) //первый символ подошел
		{
			while (j != len)
			{
				if (buffer[i + 1] != keyword[j]) //не подошло
				{
					j = 1;
					ret = -1;
					goto END;
				}
				i++;
				j++;
			}
			//всё найдено...
			i += 2;
			j = 0;
			while(1)
			{
				if (buffer[i] == '\n' || buffer[i] == '\0' || buffer[i] == '\r') break;
				dest[j] = buffer[i];
				j++;
				i++;
			}
			dest[j]='\0';
			return i;
		}
		END:
			i++;
	}
	return ret;
}

int GetStrDataFromSystemConf(char *dest, const char *path, const char *keyword)
{
	FSTATS fs;
	unsigned int err;
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	
	int fp = _open(path, A_ReadOnly, P_READ, &err);
	if (fp == -1) return -1;
	
	char *buffer = malloc(fs.size);
	if (!_read(fp, buffer, fs.size, &err))
	{
		mfree(buffer);
		_close(fp, &err);
		return -1;
	}
	
	if (Parse(dest, buffer, fs.size - 1, keyword) == -1)
	{
		mfree(buffer);
		_close(fp, &err);
		return -1;
	}
	_close(fp, &err);
	return 1;
}
