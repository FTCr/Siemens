#include <swilib.h>
#include "mplayer.h"

void Send_MPlayer_Command(unsigned short cmd, short substract)
{
	GBS_SendMessage(0x4209, 0x8053, 0, *((unsigned int*)RamMPlayer_CtrlCode())-substract,cmd);
}

void MPlayer_VolChange(short dir)
{
	int cmd;
	if(dir==1)
		cmd = MPLAYER_VOL_UP;
	else
		cmd = MPLAYER_VOL_DN;
	Send_MPlayer_Command(cmd, 2);
	Send_MPlayer_Command(cmd, 0);
}

void MPlayer_Stop(void)     {Send_MPlayer_Command(MPLAYER_STOP, 0);}
void MPlayer_Start(void)    {Send_MPlayer_Command(MPLAYER_PLAY, 0);}
void MPlayer_Next(void)     {Send_MPlayer_Command(MPLAYER_NEXT, 0);}
void MPlayer_Prev(void)     {Send_MPlayer_Command(MPLAYER_PREV, 0);}
void MPlayer_Toggle(void)   {Send_MPlayer_Command(MPLAYER_TOGGLE, 0);}
void MPlayer_Shutdown(void) {Send_MPlayer_Command(MPLAYER_KILL, 0);}
void MPlayer_Mute(void)     {Send_MPlayer_Command(MPLAYER_MUTE, 0);}
void MPlayer_Pause(void)    {Send_MPlayer_Command(MPLAYER_PAUSE, 0);}
