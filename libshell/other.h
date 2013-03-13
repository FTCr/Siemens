#ifndef _LIBSHELL_OTHER_H_
	#define _LIBSHELL_OTHER_H_
	
extern char root_dir[128];
extern char conf_dir[128];
extern char img_dir[128];
extern char lang_dir[128];

extern unsigned int shell_gui_id;
extern unsigned int cur_desk_id;
extern unsigned int desk_total;
extern unsigned int keyblock_id;

int Reload(void);
void Close(void);

void InitPath(void);

#endif
