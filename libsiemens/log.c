#include <swilib.h>

#define ACCESS A_WriteOnly + A_Create + A_BIN

char PATH[128];

int log_init(const char *path)
{
	unsigned int err;
	strcpy(PATH, path);
	int fp = _open(PATH, ACCESS, P_WRITE + P_READ, &err);
	if (fp == -1) return -1;
	
	TDate td;
	TTime tm;
	GetDateTime(&td, &tm);
	
	char data[32];
	sprintf(data, "[%d.%d.%d_%d:%d]\n", td.year, td.month, td.day, tm.hour, tm.min);
	_write(fp, data, strlen(data), &err);
	_close(fp, &err);
	
	return 0;
}

int log_integer(const char *prev_text, const int data)
{
	unsigned int err;
	int fp = _open(PATH, ACCESS, P_WRITE + P_READ, &err);
	if (fp == -1) return -1;
	
	const unsigned int Len=strlen(prev_text) + 1 + 32;
	char *log_out = malloc(Len);
	sprintf(log_out, "%s: %d;\n", prev_text, data);
	_write(fp, log_out, Len, &err);
	mfree(log_out);
	_close(fp, &err);
	
	return 0;
}
