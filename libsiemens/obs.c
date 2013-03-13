#include <swilib.h>
#include "obs.h"
#include "string_utils.h"

unsigned int GetExtUid(const char *ext)
{
	unsigned int uid = 0;
	
	unsigned int len = strlen(ext);
	
	if (!len) return uid;
	
	char *extension = malloc(strlen(ext));
	str2lower(extension, ext);
	
	inline int func(const char *ext_func)
	{
		if (strcmp(extension, ext_func) == 0) return 1;
		else return 0;
	}
	
	if (func("mp3"))
		uid = UID_MP3;
	else if (func("m3u"))
		uid = UID_M3U;
	else if (func("jar"))
		uid = UID_JAR;
	else if (func("jad"))
		uid = UID_JAD;
	else if (func("mid"))
		uid = UID_MID;
	else if (func("amr"))
		uid = UID_AMR;
	else if (func("imy"))
		uid = UID_IMY;
	else if (func("srt"))
		uid = UID_SRT;
	else if (func("aac"))
		uid = UID_AAC;
	else if (func("wav"))
		uid = UID_WAV;
	else if (func("jts"))
		uid = UID_JTS;
	else if (func("xmf"))
		uid = UID_XMF;
	else if (func("m4a"))
		uid = UID_M4A;
	else if (func("bmx"))
		uid = UID_BMX;
	else if (func("wbmp"))
		uid = UID_WBMP;
	else if (func("bmp"))
		uid = UID_BMP;
	else if (func("jpg"))
		uid = UID_JPG;
	else if (func("jpeg"))
		uid = UID_JPG;
	else if (func("png"))
		uid = UID_PNG;
	else if (func("gif"))
		uid = UID_GIF;
	else if (func("svg"))
		uid = UID_SVG;
	else if (func("3gp"))
		uid = UID_3GP;
	else if (func("mp4"))
		uid = UID_M4A;
	else if (func("sdp"))
		uid = UID_SDP;
	else if (func("pvx"))
		uid = UID_PVX;
	else if (func("sdt"))
		uid = UID_SDT;
	else if (func("ldb"))
		uid = UID_LDB;
	else if (func("txt"))
		uid = UID_TXT;
	else if (func("url"))
		uid = UID_URL;
	
	mfree(extension);
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

void SetScaling(HObj obj, short *w, short *h, short width, short height)
{
	float pct = 0;
	short _w = *w, _h = *h;
	if (_w < _h)
	{
		if (_h > height) goto HORI;
		VERT:
			pct = 100 / ((float)_h / height);
			_h = height;
			_w = _w * pct / 100;
	}
	else
	{
		if (_w > width) goto VERT;
		HORI:
		pct = 100 / ((float)_w / width);
		_w = width;
		_h = _h * pct / 100;
	}
	*w = _w;
	*h = _h;
	Obs_SetScaling(obj, 3);
}
