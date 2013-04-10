#include <swilib.h>
#include "files.h"

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
