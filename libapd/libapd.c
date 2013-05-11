#include <swilib.h>
#include <libid3.h>
#include <libapd.h>
#include "../libsiemens/other.h"
#include "../libsiemens/obs.h"
#include "../libsiemens/find_files.h"

#define WAY_NEXT 0x00
#define WAY_PREV 0x01

static DIR_ENTRY_LIST *top;      //список композиций
static DIR_ENTRY_LIST cur_track_data;

static HObj obj;

static char dir_mus[128];
static char dir_pls[128];

static int volume;
static int play_status;
static int track_id;
static int track_pos_ms;
static int track_dur_ms;
static int is_muted;
static int is_launch;
static int playback;

static int track_id_prev; // запоминаем предыдущий трэк для рандома, чтобы одинаковые трэки подряд не гонял
static unsigned int rand_seed;

static GBSTMR tmr_start_playing;
static GBSTMR tmr_auto_switch;
static GBSTMR tmr_play_after;

static IPC_REQ ipc;

static void AutoSwitcher(void)
{
	if (track_dur_ms - track_pos_ms <= 1000)
	{
		GBS_DelTimer(&tmr_auto_switch);
		APlayer_NextTrack();
	}
	else
	{
		track_pos_ms += 1000;
		GBS_StartTimerProc(&tmr_auto_switch, TMR_6_SEC / 6, (void*)AutoSwitcher);
	}
	IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_POS, 0);
}

static void Play(void)
{
	//получаем длину трэка
	GetPlayObjDuration((void*)obj, &track_dur_ms);
	if (track_dur_ms) //как только получили дляну трэка запускаем воспроизведение*/
	{
		GBS_DelTimer(&tmr_start_playing);
		Obs_Start(obj);
		IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_DUR, 0);
		AutoSwitcher();
	}
	else
	{
		GBS_StartTimerProc(&tmr_start_playing, TMR_6_SEC / 6 / 2 / 2, (void*)Play);
	}
}

static void InitTrackData(void)
{
	IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_TRACK, 0);
	IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_DUR, 0);
	IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_POS, 0);
	
	DIR_ENTRY_LIST *ptr = GetDEListPtr(top, track_id);
	memcpy(&cur_track_data, ptr, sizeof(DIR_ENTRY_LIST));
}

static void ChangeTrack(int way)
{
	switch (playback)
	{
		case APLAYER_PLAYBACK_REPEAT:
			if (way == WAY_NEXT)
			{
				if (track_id >= (int)APlayer_GetTotalTracks() - 1)
					track_id = 0;
				else
					track_id++;
			}
			else
			{
				if (track_id <= 0)
					track_id = APlayer_GetTotalTracks() - 1;
				else
					track_id--;
			}
		break;
		case APLAYER_PLAYBACK_RANDOM:
			if (APlayer_GetTotalTracks() > 2)
			{
				START:
					track_id = rand(&rand_seed) % APlayer_GetTotalTracks();
					if (track_id != track_id_prev)
					{
						track_id_prev = track_id;
					}
					else goto START;
			}
			else
				track_id++;
		break;
	}
}

void APlayer_Play(void)
{
	if (play_status != APLAYER_PLAY)
	{
		if (APlayer_GetTotalTracks())
		{
			if (!obj)
			{
				DIR_ENTRY_LIST *ptr = GetDEListPtr(top, track_id);
				FSTATS fs;
				unsigned int err;
				static int count = 0;
				if (GetFileStats(ptr->path, &fs, &err) == -1)
				{
					if (count < 5)
					{
						ChangeTrack(WAY_NEXT);
						count++;
						MsgBoxError(1, (int)"Audio file not found!");
						APlayer_Play();
					}
					else
					{
						count = 0;
					}
					return;
				}
				else
				{
					count = 0;
					obj = CreateHObjFromAudioFile(ptr->path);
					InitTrackData();
					play_status = APLAYER_PLAY;
					IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_PLAYSTATUS, 0);
					Play();
				}
			}
			else
			{
				play_status = APLAYER_PLAY;
				IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_PLAYSTATUS, 0);
				Obs_Resume(obj);
				AutoSwitcher();
			}
			Obs_Sound_SetVolumeEx(obj, volume, 1);
		}
	}
}

static void BeforeChangeTrack(void)
{
	GBS_DelTimer(&tmr_play_after);
	APlayer_Play();
}

void APlayer_PlayTrackID(unsigned int id)
{
	if (id < APlayer_GetTotalTracks())
	{
		APlayer_Stop();
		track_id = id;
		InitTrackData();
		APlayer_Play();
	}
}

void APlayer_NextTrack(void)
{
	APlayer_Stop();
	if (play_status != APLAYER_PLAY)
	{
		if (APlayer_GetTotalTracks())
		{
			ChangeTrack(WAY_NEXT);
			InitTrackData();
			GBS_StartTimerProc(&tmr_play_after, TMR_6_SEC / 6 / 3, (void*)BeforeChangeTrack);
		}
	}
}

void APlayer_PrevTrack(void)
{
	APlayer_Stop();
	if (play_status != APLAYER_PLAY)
	{
		if (APlayer_GetTotalTracks())
		{
			ChangeTrack(WAY_PREV);
			InitTrackData();
			GBS_StartTimerProc(&tmr_play_after, TMR_6_SEC / 6 / 3, (void*)BeforeChangeTrack);
		}
	}
}

static void DelTimers(void)
{
	if (IsTimerProc(&tmr_play_after))
		GBS_DelTimer(&tmr_play_after);
	if (IsTimerProc(&tmr_start_playing))
		GBS_DelTimer(&tmr_start_playing);
	if (IsTimerProc(&tmr_auto_switch))
		GBS_DelTimer(&tmr_auto_switch);
}

void APlayer_Stop(void)
{
	DelTimers();
	if (play_status != APLAYER_STOP)
	{
		if (obj)
		{
			Obs_DestroyObject(obj);
			obj = 0;
		}
		play_status = APLAYER_STOP;
		IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_PLAYSTATUS, 0);
		track_pos_ms = 0;
		track_dur_ms = 0;
	}
}

void APlayer_Pause(void)
{
	Obs_Pause(obj);
	if (IsTimerProc(&tmr_auto_switch))
		GBS_DelTimer(&tmr_auto_switch);
	play_status = APLAYER_PAUSE;
	IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_PLAYSTATUS, 0);
}

void APlayer_Toggle(void)
{
	if (play_status == APLAYER_PLAY)
		APlayer_Pause();
	else
		APlayer_Play();
}

void APlayer_SetVolume(unsigned int vol)
{
	if (vol <= 15)
	{
		Obs_Sound_SetVolumeEx(obj, vol, 1);
		volume = vol;
		IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_VOL, 0);
	}
}

void APlayer_MutedToggle(void)
{
	if (is_muted == 0)
	{
		is_muted = 1;
		Obs_Sound_SetVolumeEx(obj, 0, 1);
	}
	else
	{
		Obs_Sound_SetVolumeEx(obj, volume, 1);
		is_muted = 0;
	}
}

static void After(void)
{
	GBS_DelTimer(&tmr_play_after);
	Obs_SetPosition(obj, track_pos_ms);
	APlayer_Play();
}

void APlayer_Forward(unsigned int inc_s)
{
	if (track_dur_ms)
	{
		if (play_status != APLAYER_FORWARD)
		{
			APlayer_Pause();
		}
		track_pos_ms += inc_s * 1000;
		play_status = APLAYER_FORWARD;
		IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_PLAYSTATUS, 0);
		if (track_pos_ms > track_dur_ms)
		{
			if (IsTimerProc(&tmr_play_after))
				GBS_DelTimer(&tmr_play_after);
			APlayer_NextTrack();
		}
		else
		{
			IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_POS, 0);
			GBS_StartTimerProc(&tmr_play_after, TMR_6_SEC / 6 / 3, (void*)After);
		}
	}
}

void APlayer_Rewind(unsigned int dec_s)
{
	if (track_dur_ms)
	{
		if (play_status != APLAYER_REWIND)
		{
			APlayer_Pause();
		}
		track_pos_ms -= dec_s * 1000;
		play_status = APLAYER_REWIND;
		IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_PLAYSTATUS, 0);
		if (track_pos_ms < 0)
		{
			if (IsTimerProc(&tmr_play_after))
				GBS_DelTimer(&tmr_play_after);
			APlayer_PrevTrack();
		}
		else
		{
			IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_UPDATE_POS, 0);
			GBS_StartTimerProc(&tmr_play_after, TMR_6_SEC / 6 / 3, (void*)After);
		}
	}
}

void APlayer_SetPlayBack(unsigned int playback_mode)
{
	if (playback_mode <= APLAYER_PLAYBACK_RANDOM)
		playback = playback_mode;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

unsigned int APlayer_GetVolume(void){return volume;}

unsigned int APlayer_GetPlayStatus(void){return play_status;}

unsigned int APlayer_GetTrack(void){return track_id + 1;}

unsigned int APlayer_GetTotalTracks(void){return GetDEListTotalItems(top);}

unsigned int APlayer_GetTrackDuration(void){return track_dur_ms;}

unsigned int APlayer_GetTrackPosition(void){return track_pos_ms;}

unsigned int APlayer_IsMuted(void){return is_muted;}

unsigned int APlayer_GetPlayBack(void){ return playback;}

char *APlayer_GetPlsDir(void){return dir_pls;}

////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

DIR_ENTRY_LIST *APlayer_GetPtr(unsigned int n)
{
	return GetDEListPtr(top, n);
}

DIR_ENTRY_LIST *APlayer_GetCurPtr()
{
	return &cur_track_data;
}

static DIR_ENTRY_LIST *buffer;

void APlayer_CutFile(unsigned int n)
{
	CutDEListData(&top, &buffer, n);
	if (!APlayer_GetTotalTracks())
	{
		APlayer_Stop();
		zeromem(&cur_track_data, sizeof(DIR_ENTRY_LIST));
	}
}

void APlayer_PasteFile(unsigned int n)
{
	PasteDEListData(&top, &buffer, n);
}

int APlayer_SavePlayList(const char *name)
{
	char path[256];
	strcpy(path, dir_pls);
	strcat(path, name);
	strcat(path, ".m3u");
	
	#define M3U_HEADER  "#EXTM3U"
	#define END_OF_LINE "\r\n"
	FSTATS fs;
	unsigned int err;
	
	if (!top) return -1;
	
	if (GetFileStats(path, &fs, &err) != -1) return -1;
	
	int fp = _open(path, A_WriteOnly + A_TXT + A_Append + A_Create, P_WRITE, &err);
	if (fp == -1) return -1;
	
	_write(fp, M3U_HEADER, 7, &err);
	_write(fp, END_OF_LINE, 2, &err);
	
	DIR_ENTRY_LIST *p = top;
	unsigned int len;
	while(p)
	{
		len = strlen(p->path);
		_write(fp, p->path, len, &err);
		_write(fp, END_OF_LINE, 2, &err);
		p = p->next;
	}
	_close(fp, &err);

	return 1;
}

int APlayer_OpenPlayList(const char *path)
{
	
	FSTATS fs;
	unsigned int err;
	
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	
	int fp = _open(path, A_ReadOnly + A_BIN, P_READ, &err);
	if (fp == -1) return -1;
	
	char *buffer = malloc(fs.size);
	_read(fp, buffer, fs.size, &err);
	buffer[fs.size] = '\0';

	DIR_ENTRY_LIST *prev;
	DIR_ENTRY_LIST *m3u_list  = malloc(sizeof(DIR_ENTRY_LIST));
	DIR_ENTRY_LIST *m3u_top = m3u_list;
	char file_path[256];
	unsigned int i = 0, j = 0;
	unsigned int total = 0;	
	
	#define func(x) buffer[x] != '\n' && buffer[x] !='\0'
	while (i < fs.size)
	{
		if (func(i))
		{
			if (buffer[i] == '#')
			{
				while(func(i))
					i++;
			}
			else
			{
				if (buffer[i] != '\r')
					file_path[j++] = buffer[i];
			}
		}
		else
		{
			file_path[j] = '\0';
			j = 0;
			
			strcpy(m3u_list->path, file_path);
			
			ID3 *id3 = malloc(sizeof(ID3));
			if (GetID3(id3, m3u_list->path) > 0)
			{
				m3u_list->data = id3;
			}
			else
			{
				mfree(id3);
				m3u_list->data = NULL;
			}
			
			
			
			prev = m3u_list;
			m3u_list->next = malloc(sizeof(DIR_ENTRY_LIST));
			m3u_list = m3u_list->next;
			m3u_list->prev = prev;
			
			
			total++;
		}
		i++;
	}
	_close(fp, &err);
	if (total)
	{
		m3u_top->prev = NULL;
		m3u_list = prev;
		mfree(m3u_list->next);
		m3u_list->next = NULL;
	}
	else
	{
		mfree(m3u_list);
		return -1;
	}
	
	unsigned int n = GetDEListTotalItems(top);
	
	if (n)
	{
		DIR_ENTRY_LIST *p = GetDEListPtr(top, n - 1);
		p->next = m3u_top;
		m3u_top->prev = p;
	}
	else
	{
		top = m3u_top;
	}
	return 1;
}


int APlayer_OpenFile(const char *path)
{
	FSTATS fs;
	unsigned int err;
	if (GetFileStats(path, &fs, &err) == -1) return -1;
	
	DIR_ENTRY_LIST *p = top;
	DIR_ENTRY_LIST *prev;
	if (p)
	{
		while(1)
		{
			if (p->next) p = p->next;
			else break;
		}
		prev = p;
		p->next = malloc(sizeof(DIR_ENTRY_LIST));
		p = p->next;
	}
	else
	{
		top = malloc(sizeof(DIR_ENTRY_LIST));
		p = top;
		prev = NULL;
	}
	p->next = NULL;
	p->prev = prev;
	strcpy(p->path, path);
	
	ID3 *id3 = malloc(sizeof(ID3));
	if (GetID3(id3, p->path) > 0)
	{
		p->data = id3;
	}
	else
	{
		mfree(id3);
		p->data = NULL;
	}
	return 1;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


static void CallBackFind(DIR_ENTRY_LIST *ptr)
{
	ID3 *id3 = malloc(sizeof(ID3));
	if (GetID3(id3, ptr->path) > 0)
	{
		ptr->data = id3;
	}
	else
	{
		mfree(id3);
		ptr->data = NULL;
	}
}

static void CallBackFree(DIR_ENTRY_LIST *ptr)
{
	ID3 *id3 = ptr->data;
	if (id3)
	{
		FreeID3(id3);
		mfree(id3);
	}
}

void APlayer_ClearPlayList(void)
{
	APlayer_Stop();
	if (top)
	{
		FreeDEList(top, CallBackFree);
		top = NULL;
		zeromem(&cur_track_data, sizeof(DIR_ENTRY_LIST));
	}
	track_id = 0;
}

void APlayer_FindMusic(void)
{
	APlayer_ClearPlayList();
	
	FIND_UIDS fu;
	fu.uid1 = UID_MP3;
	fu.uid2 = UID_WAV;
	fu.uid3 = UID_AAC;
	FindFilesRec(&top, dir_mus, &fu, CallBackFind);
	zeromem(&cur_track_data, sizeof(DIR_ENTRY_LIST));
}

unsigned int APlayer_Init(const char *mus_dir, const char *pls_dir)
{
	if (!is_launch)
	{
		strcpy(dir_mus, mus_dir);
		strcpy(dir_pls, pls_dir);
		APlayer_FindMusic();
		SettingsAE_Read(&volume, AE_MEDIAPLAYER, NULL, "VOLUME");
		
		int tmp_playback;
		SettingsAE_Read(&tmp_playback, AE_MEDIAPLAYER, NULL, "PLAYMODE");
		if (tmp_playback == 5)
			playback = APLAYER_PLAYBACK_RANDOM;
		else
			playback = APLAYER_PLAYBACK_REPEAT;
		
		is_launch = 1;
		srand(&rand_seed);
	}
	return (APlayer_GetTotalTracks()) ? 1 : 0;
}

void APlayer_Destroy(void)
{
	APlayer_ClearPlayList();
	
	if (buffer) mfree(buffer);
	SettingsAE_Update(volume, AE_MEDIAPLAYER, NULL, "VOLUME");
	
	int tmp_playback = (playback == APLAYER_PLAYBACK_REPEAT) ? 6 : 5;
	SettingsAE_Update(tmp_playback, AE_MEDIAPLAYER, NULL, "PLAYMODE");
	
	is_launch = 0;
	IPC_SendMessage(&ipc, APD_IPC_NAME, "libapd", APD_IPC_DESTROY, 0);
}

unsigned int APlayer_IsLaunch(void){return is_launch;}
