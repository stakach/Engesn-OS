#ifndef _devices_H_
#define _devices_H_

#include "kerndefs.h"
#include "cmos.h"

void deviceSetup();		/* Detects installed drives */
void printDevices(int window); /* Prints drive list to a window */

extern unsigned char floppy[1]; /* Shows which floppy drives are installed */
extern unsigned int convmem_size, extmem_size;

#endif
