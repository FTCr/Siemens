#include <swilib.h>
#include "strings.h"

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
