#ifndef _LIBS_OBS_H_
	#define _LIBS_OBS_H_

#define UID_MP3  0x03
#define UID_M3U  0x04
#define UID_JAR  0x12
#define UID_JAD  0x13
#define UID_MID  0x18
#define UID_AMR  0x19
#define UID_IMY  0x1A
#define UID_SRT  0x1C
#define UID_AAC  0x1D
#define UID_WAV  0x1E
#define UID_JTS  0x20
#define UID_XMF  0x21
#define UID_M4A  0x22
#define UID_BMX  0x23
#define UID_WBMP 0x24
#define UID_BMP  0x25
#define UID_JPG  0x26
#define UID_PNG  0x27
#define UID_GIF  0x28
#define UID_SVG  0x2B
#define UID_3GP  0x32
#define UID_MP4  0x33
#define UID_SDP  0x3E
#define UID_PVX  0x3F
#define UID_SDT  0x40
#define UID_LDB  0x44
#define UID_TXT  0x57
#define UID_URL  0x58

unsigned int GetExtUidByFileName(const char *path);

HObj CreateHObjFromImgFile(const char *path);
IMGHDR *HObj2IMGHDR(HObj obj);
void SetScaling(HObj obj, short *w, short *h, short width, short height);

HObj CreateHObjFromAudioFile(char *path);
unsigned int Obs_SoundGetVolume(HObj obj);

#endif
