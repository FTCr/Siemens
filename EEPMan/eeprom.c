#include <swilib.h>

int EELiteWriteBlock(unsigned int block, void *data, unsigned int size, char ver)
{
	StartWriteEELiteBlock(block, size, ver, NULL);
	WriteEELiteBlock(block, data, 0, size, NULL);
	FinishWriteEELiteBlock(block, NULL);
	return 1;
}

int EELiteReadBlock(int block, void *buf, int offset, int size)
{
	void *data = GetEELiteBlockAddress(block);
	if (data == NULL) return -1;
	memcpy(buf, data + offset, size - offset);
	return 1;
}