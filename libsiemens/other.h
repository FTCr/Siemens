#ifndef _LIBSIEMENS_OTHER_H_
	#define _LIBSIEMENS_OTHER_H_

//отправка ipc мессаги
void IPC_SendMessage(IPC_REQ *ipc, const char *name_to, const char *name_from, const int mess, void *data);
//запуск энтрипоинта
void ExecEntrypoint(const char *name);
//запуск шорката
void ExecShortcut(const char *name);
//запуск файла
void ExecFile(const char *path);
//получение csm_id из адреса
unsigned int GetCSMIDFromAddr(const char *addr);

//установка хука на вызов фукнции, должна быть подргужена swi.blib
int *SetSWIHook(int swi_num, void *proc);
//снятие хука
void DestroySWIHook(int swi_num, unsigned int *addr);

void patch_rect(RECT*rc, int x, int y, int x2, int y2);
void patch_header(HEADER_DESC* head);
void patch_header_small(HEADER_DESC* head);
void patch_input(INPUTDIA_DESC* inp);

#endif
