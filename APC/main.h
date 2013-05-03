#ifndef _MAIN_H_
	#define _MAIN_H_

extern char **lgp;

enum
{
	lgpHeader,
	
	lgpOptions,
	lgpExit,
	lgpReplace,
	
	
	
	//lgpOptionsHeader,
	
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
	//lgpOptionsTrackInfo,
	lgpOptionsHelp,
	lgpOptionsAbout,
	
	/*lgpTrackInfoTitle,
	lgpTrackInfoArtist,
	lgpTrackInfoAlbum,
	lgpTrackInfoTrack,
	lgpTrackInfoGenre,*/
	
	lgpPlayBackRepeat,
	lgpPlayBackRando,
	
	lgpPlaylistSaveHead,
	lgpPlaylistSave,
	
	lgpDelQ
};

void RefreshMainGUI(void);

#endif
