#include <swilib.h>
#include "../libsiemens/graphics.h"
#include "../libsiemens/cfg.h"
#include "../libsiemens/files.h"
#include "../libsiemens/other.h"
#include "../libsiemens/ipc.h"
#include "../libsiemens/lang.h"
#include "../libsiemens/obs.h"
#include "../libshell/other.h"
#include "../libshell/graphics.h"
#include "../libshell/plugins.h"
#include "../libshell/conf_loader.h"
#include "../libshell/config_data.h"

typedef struct
{
	GUI gui;
} MAIN_GUI;

void DestroyIdleHook(void);

static void OnRedraw(MAIN_GUI *data)
{
	DrawBG();
	if (plg)
	{
		for (int i = 0; plg[i] != NULL; i++)
			if (plg[i]->OnRedraw && IsUsePlg(plg[i])) plg[i]->OnRedraw();
	}
}

static void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	data->gui.state = 1;
	if (plg)
	{
		for (int i = 0; plg[i] != NULL; i++)
			if (plg[i]->OnCreate && IsUsePlg(plg[i])) plg[i]->OnCreate();
	}
}

static void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	if (plg)
	{
		for (int i = 0; plg[i] != NULL; i++)
			if (plg[i]->OnClose && IsUsePlg(plg[i])) plg[i]->OnClose();
	}
	shell_gui_id = data->gui.state = 0;
}

static void OnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
	data->gui.state = 2;
	DisableIDLETMR();
	if (plg)
	{
		for (int i = 0; plg[i] != NULL; i++)
			if (plg[i]->OnFocus && IsUsePlg(plg[i])) plg[i]->OnFocus();
	}
}

static void OnUnFocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	if (data->gui.state != 2) return;
	data->gui.state = 1;

	if (plg)
	{
		for (int i = 0; plg[i] != NULL; i++)
			if (plg[i]->OnUnFocus && IsUsePlg(plg[i])) plg[i]->OnUnFocus();
	}
}

int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
	const unsigned int Key  =  msg->gbsmsg->submess;
	const unsigned int Type = msg->gbsmsg->msg;

	if (plg)
	{
		if (!keyblock_id)
		{
			for (int i = 0; plg[i] != NULL; i++)
			{
				if (keyblock_id) //кто-то не хочет больше обработчиков
					break;
				if (plg[i]->OnKey && IsUsePlg(plg[i])) plg[i]->OnKey(Key, Type);
			}
		}
		else
		{
			for (int i = 0; plg[i] != NULL; i++)
			{
				if (plg[i]->OnKey && IsUsePlg(plg[i]) && plg[i]->id == keyblock_id)
				{
					plg[i]->OnKey(Key, Type);
					break;
				}
			}
		}
	}
	return 0;
}

extern void kill_data(void *p, void (*func_p)(void *));

static int Method8(void){return 0;}
static int Method9(void){return 0;}


static const void * const gui_methods[11] =
{
	(void*)OnRedraw,
	(void*)OnCreate,
	(void*)OnClose,
	(void*)OnFocus,
	(void*)OnUnFocus,
	(void*)OnKey,
	0,
	(void*)kill_data,
	(void*)Method8,
	(void*)Method9,
	0
};

void CreateShellGUI(void)
{
	static RECT Canvas = {0,0,0,0};
	MAIN_GUI *main_gui = malloc(sizeof(MAIN_GUI));
	zeromem(main_gui, sizeof(MAIN_GUI));
	Canvas.x2 = ScreenW() - 1;
	Canvas.y2 = ScreenH() - 1;
	main_gui->gui.canvas             = (RECT *)(&Canvas);
	main_gui->gui.methods            = (void *)gui_methods;
	main_gui->gui.item_ll.data_mfree = (void (*)(void *))mfree_adr();
	shell_gui_id = CreateGUI(main_gui);
}



int (*OldOnMessage)(CSM_RAM*, GBS_MSG *msg);

CSM_RAM *GetIdleCSM(void)
{
	return (CSM_RAM*)FindCSMbyID(CSM_root()->idle_id);
}

int OnMessage(CSM_RAM *data, GBS_MSG *msg)
{
	if (msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp(cfg_path, (char *)msg->data0) == 0)
			Reload();
		if (strcmp(cfg_col_path, (char*)msg->data0) == 0)
			InitConfigCol();
		if (strcmp(cfg_font_path, (char*)msg->data0) == 0)
			InitConfigFont();
		if (strcmp(cfg_coord_path, (char*)msg->data0) == 0)
			InitConfigCoord();
	}
	if (plg)
	{
		//мессаги принимаются независимо от рабочего стола
		for (int i = 0; plg[i] != NULL; i++)
			if(plg[i]->OnMessage) plg[i]->OnMessage(data, msg);
		//а эти зависимо
		for (int i = 0; plg[i] != NULL; i++)
			if (plg[i]->OnMessageDep && IsUsePlg(plg[i])) plg[i]->OnMessageDep(data, msg);
	}
	if (msg->msg == MSG_IPC)
	{
		IPC_REQ *ipc = (IPC_REQ*)msg->data0;
		if (ipc)
		{
			if (strcmp(ipc->name_to, "Shell") == 0)
			{
				switch (msg->submess)
				{
					case IPC_CLOSE:
						DestroyIdleHook();
						if (shell_gui_id)
							GeneralFunc_flag1(shell_gui_id, 0);
						UploadGraphics();
						UploadPlugins();
						kill_elf();
					break;
					case IPC_RELOAD:
						UploadGraphics();
						InitConfig();
						InitPath();
						if (LoadGraphics() == -1)
						{
							static IPC_REQ ipc;
							IPC_SendMessage(&ipc, "Shell", "Shell", IPC_CLOSE, NULL);
						}
						else
						{
							UploadPlugins();
							cur_desk_id = 1;
							desk_total  = 0;
							LoadPlugins();
							DirectRedrawGUI_ID(shell_gui_id);
						}
					break;
				}
			}
		}
	}
	return 1;
}


int IdleCSMOnMessage(CSM_RAM *csm, GBS_MSG *msg)
{
	#define IDLEGUI_ID (((int*)csm)[DISPLACE_OF_IDLEGUI_ID/4])

	int on_mes = OnMessage(csm, msg);
	if (IsGuiOnTop(IDLEGUI_ID))
	{
		GUI *igui = GetTopGUI();
		if (igui)
			CreateShellGUI();
	}
	on_mes = OldOnMessage(csm, msg);
	return on_mes;
}

void DestroyIdleHook(void)
{
	CSM_RAM *csm = GetIdleCSM();
	((CSM_DESC*)csm->constr)->onMessage = OldOnMessage;
}

int main(const char *bin_path, const char *fname)
{
	//корневой каталог
	char *ptr = strrchr(bin_path, '\\');
	ptr++;
	memcpy(root_dir, bin_path, ptr - bin_path);
	
	InitConfig();
	InitPath();
	if (LoadGraphics() == -1)
	{
		UploadGraphics();
		kill_elf();
		return 0;
	}
	InitConfigCol();
	InitConfigFont();
	InitConfigCoord();
	sprintf(lang_dir, "%s%s", root_dir, "Langpack\\");
	LoadPlugins();
	LockSched();
	CSM_RAM *csm = GetIdleCSM();
	OldOnMessage = ((CSM_DESC*)csm->constr)->onMessage;
	((CSM_DESC*)(csm->constr))->onMessage = IdleCSMOnMessage;
	UnlockSched();
	return 0;
}
