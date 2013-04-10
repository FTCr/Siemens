#ifndef _LIBSIEMENS_OBSS_H_
	#define _LIBSIEMENS_OBS_H_

HObj CreateHObjFromImgFile(const char *path);
//получени IMGHDR* из объекта
IMGHDR *HObj2IMGHDR(HObj obj);
//создание объекта из аудиофайла
HObj CreateHObjFromAudioFile(char *path);
//установка громкости проигрывания аудиофайла
unsigned int Obs_SoundGetVolume(HObj obj);

#endif
