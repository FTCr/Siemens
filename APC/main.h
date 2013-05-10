#ifndef _MAIN_H_
	#define _MAIN_H_

extern char **lgp;

enum
{
	lgpHeader,
	
	lgpOptions,
	lgpExit,
	lgpReplace,
	
	lgpOptionsSelect,
	lgpOptionsBack,
	
	lgpOptionsPlay,
	lgpOptionsStop,
	
	lgpOptionsPlayMode,
	lgpOptionsCut,
	lgpOptionsPaste,
	lgpOptionsClear,
	lgpOptionsPlaylistSave,
	lgpOptionsSend,
	lgpOptionsDel,
	lgpOptionsHelp,
	lgpOptionsAbout,
	
	lgpPlayBackRepeat,
	lgpPlayBackRando,
	
	lgpPlaylistSaveHead,
	lgpPlaylistSave,
	
	lgpDelQ,
	
	lgpInfoOK,
	lgpInfoBack
};

void RefreshMainGUI(void);

#endif
