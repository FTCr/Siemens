#include <swilib.h>

void IPC_SendMessage(const char *name_to, const char *name_from, const int mess)
{
	static IPC_REQ ipc;
	ipc.name_to   = name_to;
	ipc.name_from = name_from;
	ipc.data      = 0;
	GBS_SendMessage(MMI_CEPID, MSG_IPC, mess, &ipc);
}
