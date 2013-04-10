#ifndef _LIBSIEMENS_LANG_H_
	#define _LIBSIEMENS_LANG_H_

//загрузка лэнгпака из текстового файла
int InitLang(const char *path, char ***lgp);
//соотвественно очистка
void FreeLang(char ***lgp);

#endif
