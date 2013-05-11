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
	lgpOptionsTrackInfo,
	lgpOptionsHelp,
	lgpOptionsAbout,
	
	lgpPlayBackRepeat,
	lgpPlayBackRando,
	
	lgpPlaylistSaveHead,
	lgpPlaylistSave,
	
	lgpTagTitle,
	lgpTagArtist,
	lgpTagAlbum,
	lgpTagTrack,
	lgpTagYear,
	lgpTagComment,
	lgpTagGenre,
	
	lgpDelQ,
	
	lgpInfoOK,
	lgpInfoBack
};

void RefreshMainGUI(void);

#endif
