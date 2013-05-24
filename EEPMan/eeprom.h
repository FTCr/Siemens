#ifndef _EEPROM_H_
	#define _EEPROM_H_

int EELiteWriteBlock(unsigned int block, void *data, unsigned int size, char ver);
int EELiteReadBlock(int block, void *buf, int offset, int size);

#endif

