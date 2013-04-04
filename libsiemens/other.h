#ifndef _LIBS_OTHER_H_
	#define _LIBS_OTHER_H_

void ExecEntrypoint(const char *name);
void ExecShortcut(const char *name);
void ExecFile(const char *path);
	
unsigned int GetCSMIDFromAddr(const char *addr);
#endif
