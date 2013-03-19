#ifndef _LIBSHELL_PLUGINS_H_
	#define _LIBSHELL_PLUGINS_H_

typedef struct
{
	char fname[64]; //имя файла без расширения
	//id плагина
	unsigned int id;
	/*стандартные методы*/
	void (*OnRedraw)(void);
	void (*OnCreate)(void);
	void (*OnClose)(void);
	void (*OnKey)(unsigned int key, unsigned int type);
	void (*OnFocus)(void);
	void (*OnUnFocus)(void);
	//не зависит от рабочего стола
	void (*OnMessage)(CSM_RAM *data, GBS_MSG *msg);
	//зависит от рабочего стола
	void (*OnMessageDep)(CSM_RAM *data, GBS_MSG *msg);

	//действия при выгрузке плагина(например очистить память от графики)
	void (*Destroy)(void);
	//id рабочего стола
	unsigned int desk_id;
} PLUGIN_S4T;

extern PLUGIN_S4T **plg; //использовать с великой осторожностью

int LoadPlugins(void);
void UploadPlugins(void);
unsigned int IsUsePlg(const PLUGIN_S4T *plg);

#endif
