#include <swilib.h>

void IPC_SendMessage(IPC_REQ *ipc, const char *name_to, const char *name_from, const int mess, void *data)
{
	ipc->name_to   = name_to;
	ipc->name_from = name_from;
	ipc->data      = data;
	GBS_SendMessage(MMI_CEPID, MSG_IPC, mess, ipc);
}
