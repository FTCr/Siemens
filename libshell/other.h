#ifndef _LIBSHELL_OTHER_H_
	#define _LIBSHELL_OTHER_H_

#define IPC_CLOSE    0x00
#define IPC_KILL_ELF 0x01
	
extern char root_dir[128];
extern char conf_dir[128];
extern char img_dir[128];
extern char lang_dir[128];

extern unsigned int shell_gui_id;
extern unsigned int cur_desk_id;
extern unsigned int desk_total;
extern unsigned int keyblock_id;
extern unsigned int killed;

void InitPath(void);
void Close(void);
void Reload(void);

#endif
