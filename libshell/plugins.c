#include <swilib.h>
#include "../libsiemens/files.h"
#include "plugins.h"
#include "other.h"

PLUGIN_S4T **plg;

int LoadPlugins(void)
{
	DIR_ENTRY **de;
	char path[128];
	unsigned int total;
	strcpy(path, root_dir);
	strcat(path, "Plugins\\");

	total = FindFiles(&de, path, "*.so");

	if (de == NULL) return -1;
	
	DE_Sort1(&de);
	
	unsigned int i = 0;
	unsigned int n = 0;
	unsigned int len = 0;
	int plg_id;
	while (de[i] != NULL)
	{
		sprintf(path, "%s%s%s", root_dir, "Plugins\\", de[i]->file_name);
		plg_id = _dlopen(path);
		if (plg_id != -1)
		{
			plg        = realloc(plg, sizeof(PLUGIN_S4T*) * (n + 2));
			plg[n]     = malloc(sizeof(PLUGIN_S4T));
			plg[n + 1] = NULL;
			zeromem(plg[n], sizeof(PLUGIN_S4T));
			plg[n]->id = plg_id; // устанавливаем идентификатор для плагина
			GetFNameWithoutExt(plg[n]->fname, de[i]->file_name);
			int (*init)(PLUGIN_S4T*) = (int(*)(PLUGIN_S4T*))_dlsym(n, "main");
			if (init(plg[n]) == -1) //что-то не так, выгружаем
			{
				if (plg[n]->Destroy)
					plg[n]->Destroy();
				plg_id = plg[n]->id;
				mfree(plg[n]);
				plg[n] = NULL;
				plg = realloc(plg, sizeof(PLUGIN_S4T*) * (n + 1));
				_dlclose(plg_id);
			}
			else
				n++;
		}
		i++;
	}
	DE_Free(&de);
	return 0;
}

void UploadPlugins(void)
{
	if (plg)
	{
		unsigned int i = 0;
		i = 0;
		while(plg[i])
		{
			if (plg[i]->Destroy)
				plg[i]->Destroy();
			_dlclose(plg[i]->id);
			mfree(plg[i]);
			plg[i++] = NULL;
		}
		mfree(plg);
		plg = NULL;
	}
}

unsigned int IsUsePlg(const PLUGIN_S4T *plg)
{
	return (plg->desk_id == 0 || plg->desk_id == cur_desk_id) ? 1 : 0;
}
