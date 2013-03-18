#include "swilib.h"
#include "../libsiemens/graphics.h"
#include "../libsiemens/ipc.h"
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

void Close(void)
{
	IPC_SendMessage("Shell", "Shell", IPC_CLOSE);
}

void Reload(void)
{
	IPC_SendMessage("Shell", "Shell", IPC_RELOAD);
}
