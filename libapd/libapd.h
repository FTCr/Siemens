#ifndef _LIBAPLAYER_H_
	#define _LIBAPAYER_H_

#include "../libsiemens/find_files.h"

#define APLAYER_STOP    0x00
#define APLAYER_PAUSE   0x01
#define APLAYER_PLAY    0x02
#define APLAYER_FORWARD 0x03
#define APLAYER_REWIND  0x04

#define APLAYER_PLAYBACK_REPEAT 0x00
#define APLAYER_PLAYBACK_RANDOM 0x01

//отсылает мессаги в зависимости от действии
#define APD_IPC_NAME   "APD_Clients"

#define APD_IPC_UPDATE_TRACK 0x00
#define APD_IPC_UPDATE_DUR   0x01
#define APD_IPC_UPDATE_POS   0x02
#define APD_IPC_UPDATE_VOL   0x03
#define APD_IPC_DESTROY      0x04

void APlayer_FindMusic(void);        //обновление файлов
unsigned int APlayer_IsLaunch(void);


void APlayer_Play(void);
void APlayer_PlayTrackID(unsigned int id);
void APlayer_NextTrack(void);
void APlayer_PrevTrack(void);
void APlayer_Stop(void);
void APlayer_Pause(void);
void APlayer_Toggle(void);
void APlayer_SetVolume(unsigned int vol);
void APlayer_MutedToggle(void);
void APlayer_Forward(unsigned int inc_s);
void APlayer_Rewind(unsigned int dec_s);
void APlayer_SetPlayBack(unsigned int playback_mode);
void APlayer_ClearPlayList(void);

unsigned int APlayer_GetVolume(void);
unsigned int APlayer_GetPlayStatus(void);
unsigned int APlayer_GetTrack(void);
unsigned int APlayer_GetTotalTracks(void);
unsigned int APlayer_GetTrackDuration(void);
unsigned int APlayer_GetTrackPosition(void);
unsigned int APlayer_IsMuted(void);
unsigned int APlayer_GetPlayBack(void);
char *APlayer_GetPlsDir(void);
int APlayer_SavePlayList(const char *name);
int APlayer_OpenPlayList(const char *path);
int APlayer_OpenFile(const char *path);

DIR_ENTRY_LIST *APlayer_GetPtr(unsigned int n);
DIR_ENTRY_LIST *APlayer_GetCurPtr(void);

void APlayer_CutFile(unsigned int n);
void APlayer_PasteFile(unsigned int n);

#endif
