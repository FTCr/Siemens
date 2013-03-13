#ifndef _LIBSHELL_CONF_LOADER_H_
	#define _LIBSHELL_CONF_LOADER_H_

extern char cfg_path[128];
extern char cfg_font_path [128];
extern char cfg_col_path[128];
extern char cfg_coord_path[128];

void InitConfig(void);
void InitConfigCol(void);
void InitConfigFont(void);
void InitConfigCoord(void);

#endif
