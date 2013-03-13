#include <swilib.h>
#include <cfg_items.h>
#include "config_data.h"


char cfg_path[128];
char cfg_col_path[128];
char cfg_font_path[128];
char cfg_coord_path[128];

//__attribute__((section("CONFIG_C")))
int LoadConfigData(const char *fname, CFG_HDR *cfghdr0, int * __config_begin, int * __config_end, char *scf)
{
	int f;
	unsigned int ul;
	char *buf;
	int result = 0;
	void *cfg;

	//extern const int __config_end, __config_begin;
	cfg = (void*)cfghdr0;
	volatile unsigned int _segb = (volatile unsigned int)__config_begin;
	volatile unsigned int _sege = (volatile unsigned int)__config_end;

	unsigned int len = (_sege - _segb)-4;
	if (!(buf = malloc(len))) return -1;
	if ((f = _open(fname,A_ReadOnly+A_BIN,0,&ul)) != -1)
	{
		if (_read(f,buf,len,&ul) == (int)len)
		{
			memcpy(cfg,buf,len);
			_close(f,&ul);
		}
		else
		{
			_close(f,&ul);
			goto L_SAVENEWCFG;
		}
	}
	else
	{
		L_SAVENEWCFG:
		if ((f = _open(fname,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul)) != -1)
		{
			if (_write(f,cfg,len,&ul) != (int)len) result =- 1;
			_close(f,&ul);
		}
		else
		result=-1;
	}
	mfree(buf);
	if (result >= 0) strcpy(scf, fname);
	return(result);
}


void InitConfig(void)
{
	__CONFIG_EXTERN(0, cfghdr0);
	
	if (LoadConfigData("4:\\Zbin\\Etc\\Shell.bcfg", __CONFIG(0, cfghdr0), cfg_path) == -1)
		LoadConfigData("0:\\Zbin\\Etc\\Shell.bcfg", __CONFIG(0, cfghdr0), cfg_path);
}

void InitConfigCol(void)
{
	__CONFIG_EXTERN(1, colhdr0);

	char path[128];
	strcpy(path, cfg_skin_directory);
	strcat(path, "s-col.bcfg");
	LoadConfigData(path, __CONFIG(1, colhdr0), cfg_col_path);
}

void InitConfigFont(void)
{
	__CONFIG_EXTERN(2, fonthdr0);

	char path[128];
	strcpy(path, cfg_skin_directory);
	strcat(path, "s-font.bcfg");
	LoadConfigData(path, __CONFIG(2, fonthdr0), cfg_font_path);
}

void InitConfigCoord(void)
{
	__CONFIG_EXTERN(3, coordhdr0);

	char path[128];
	strcpy(path, cfg_skin_directory);
	strcat(path, "s-coord.bcfg");
	LoadConfigData(path, __CONFIG(3, coordhdr0), cfg_coord_path);
}
