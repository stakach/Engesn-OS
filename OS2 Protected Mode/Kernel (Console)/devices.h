#ifndef _devices_H_
#define _devices_H_

#include "kerndefs.h"
#include "cmos.h"
#include "Video.h"

class Devices
{
	public:
		Devices();		/* Detects installed drives */
		~Devices();
		void printDevices(Video &IO, int window); /* Prints drive list to a window */

		unsigned int convmem_size, extmem_size;

	private:
		void detect_floppy_drives();
		//void detect_drive_numbers();
		void detect_memory();

		unsigned char floppy[1]; /* Shows which floppy drives are installed */
		//unsigned numFD;
		//unsigned numHD;
};

#endif
