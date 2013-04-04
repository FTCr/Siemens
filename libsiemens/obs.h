#ifndef _LIBS_OBS_H_
	#define _LIBS_OBS_H_

unsigned int GetExtUidByFileName(const char *path);

HObj CreateHObjFromImgFile(const char *path);
IMGHDR *HObj2IMGHDR(HObj obj);
void SetScaling(HObj obj, short *w, short *h, short width, short height);

HObj CreateHObjFromAudioFile(char *path);
unsigned int Obs_SoundGetVolume(HObj obj);

#endif
