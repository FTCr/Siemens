#include <swilib.h>

#define PLAYER_NEXT    0x01
#define PLAYER_PREV    0x02
#define PLAYER_RESTART 0x03
#define PLAYER_MUTE    0x0A
#define PLAYER_KILL    0x0C
#define PLAYER_PLAY    0x0E
#define PLAYER_STOP    0x0F
#define PLAYER_PAUSE   0x10
#define PLAYER_TOGGLE  0x11
#define PLAYER_FWD     0x12
#define PLAYER_RWN     0x13
#define PLAYER_NRM     0x14
#define PLAYER_VOL_UP  0x15
#define PLAYER_VOL_DN  0x16
#define PLAYER_REPEAT1 0x18

void Send_MPlayer_Command(unsigned short cmd, short substract)
{
	GBS_SendMessage(0x4209, 0x8053, 0, *((unsigned int*)RamMPlayer_CtrlCode())-substract,cmd);
}

void MPlayer_VolChange(short dir)
{
	int cmd;
	if(dir==1)
		cmd = PLAYER_VOL_UP;
	else
		cmd = PLAYER_VOL_DN;
	Send_MPlayer_Command(cmd, 2);
	Send_MPlayer_Command(cmd, 0);
}

void MPlayer_Stop(void)     {Send_MPlayer_Command(PLAYER_STOP, 0);}
void MPlayer_Start(void)    {Send_MPlayer_Command(PLAYER_PLAY, 0);}
void MPlayer_Next(void)     {Send_MPlayer_Command(PLAYER_NEXT, 0);}
void MPlayer_Prev(void)     {Send_MPlayer_Command(PLAYER_PREV, 0);}
void MPlayer_Toggle(void)   {Send_MPlayer_Command(PLAYER_TOGGLE, 0);}
void MPlayer_Shutdown(void) {Send_MPlayer_Command(PLAYER_KILL, 0);}
void MPlayer_Mute(void)     {Send_MPlayer_Command(PLAYER_MUTE, 0);}
void MPlayer_Pause(void)    {Send_MPlayer_Command(PLAYER_PAUSE, 0);}
