#ifndef _LIBS_IPC_H_
	#define _LIBS_IPC_H_

#define IPC_XTASK_NAME "XTask"

#define IPC_XTASK_IDLE        0x01
#define IPC_XTASK_SHOW_CSM    0x02
#define IPC_XTASK_LOCK_SHOW   0x03
#define IPC_XTASK_UNLOCK_SHOW 0x04

void IPC_SendMessage(IPC_REQ *ipc, const char *name_to, const char *name_from, const int mess, void *data);

	
#endif
