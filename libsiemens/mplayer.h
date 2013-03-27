#ifndef _LIBS_MPLAYER_H_
	#define _LIBS_MPLAYER_H_

#define MPLAYER_NEXT    0x01
#define MPLAYER_PREV    0x02
#define MPLAYER_RESTART 0x03
#define MPLAYER_MUTE    0x0A
#define MPLAYER_KILL    0x0C
#define MPLAYER_PLAY    0x0E
#define MPLAYER_STOP    0x0F
#define MPLAYER_PAUSE   0x10
#define MPLAYER_TOGGLE  0x11
#define MPLAYER_FWD     0x12
#define MPLAYER_RWN     0x13
#define MPLAYER_NRM     0x14
#define MPLAYER_VOL_UP  0x15
#define MPLAYER_VOL_DN  0x16
#define MPLAYER_REPEAT1 0x18

void MPlayer_Stop(void);
void MPlayer_Start(void);
void MPlayer_Next(void);
void MPlayer_Prev(void);
void MPlayer_Toggle(void);
void MPlayer_Shutdown(void);
void MPlayer_Mute(void);
void MPlayer_Pause(void); 
	
#endif
