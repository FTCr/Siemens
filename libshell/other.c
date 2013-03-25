#include "swilib.h"
#include "../libsiemens/graphics.h"
#include "../libsiemens/ipc.h"
#include "../libsiemens/other.h"
#include "graphics.h"
#include "plugins.h"
#include "other.h"
#include "config_data.h"
#include "conf_loader.h"

char root_dir[128];
char conf_dir[128];
char img_dir[128];
char lang_dir[128];

unsigned int desk_total;
unsigned int cur_desk_id = 1;
unsigned int shell_gui_id;

//блокируем все обработчики, кроме выбранного
unsigned int keyblock_id;

void InitPath(void)
{
	sprintf(img_dir, "%s%s", cfg_skin_directory, "Img\\");
	sprintf(conf_dir, "%s%s", cfg_skin_directory, "Configs\\");
}

void Close(void)
{
	static IPC_REQ ipc;
	IPC_SendMessage(&ipc, "Shell", "Shell", IPC_CLOSE, NULL);
}

void Reload(void)
{
	static IPC_REQ ipc;
	IPC_SendMessage(&ipc, "Shell", "Shell", IPC_RELOAD, NULL);
}

