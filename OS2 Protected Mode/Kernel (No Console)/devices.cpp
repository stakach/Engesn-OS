

#include "devices.h"

Devices::Devices() /* Detects installed drives */
{
	//detect_drive_numbers();
	detect_floppy_drives();
	detect_memory();
}

Devices::~Devices() {}

/*void Devices::detect_drive_numbers()
{
	numHD = *(unsigned short*)0x75; /* byte at linear address 0x475 
	numFD = *(unsigned short*)0x10; /* (0x40) 16-bit word at linear address 0x410 
	if(numFD & 0x0001)
	{
		numFD = ((numFD / 64) & 3) + 1;
	}
	else
	{
		numFD = 0;
	}
}*/

void Devices::detect_floppy_drives()
{
	unsigned char c;

	outportb(0x70, 0x10);
	c = inportb(0x71);

	floppy[0] = c >> 4; /* get the high nibble */
	floppy[1] = c & 0xF; /* get the low nibble by ANDing out the high nibble */
}

void Devices::detect_memory()
{
	Cmos cmos;
	unsigned char highmem, lowmem;
	lowmem = (unsigned char)cmos.read(0x17);
		/* 0x30 is the CMOS reg that hold the low byte of the mem count */

	highmem = (unsigned char)cmos.read(0x18);
		/* 0x31 is the CMOS reg that hold the high byte of the mem count */

		/* fix the low and high bytes into one value */
	extmem_size = highmem;
	extmem_size = extmem_size << 8;
	extmem_size += lowmem;

	lowmem = (unsigned char)cmos.read(0x15);
		/* 0x30 is the CMOS reg that hold the low byte of the mem count */

	highmem = (unsigned char)cmos.read(0x16);
		/* 0x31 is the CMOS reg that hold the high byte of the mem count */

		/* fix the low and high bytes into one value */
	convmem_size = highmem;
	convmem_size = convmem_size << 8;
	convmem_size += lowmem;
}

void Devices::printDevices(Video &IO, int window) /* Prints a list of detected drives to a window */
{
	int i;
	char str[21];

/*	ntostr(numFD, str);
	IO.prints(window, "\nFD Drives Detected: ");
	IO.prints(window, str);

	ntostr(numHD, str);
	IO.prints(window, "\nHD Drives Detected: ");
	IO.prints(window, str);*/

	for(i = 0; i <= 1; i++)
	{
		IO.prints(window, "\nDrive "); /* Prints floppy drives installed */
		IO.putc(window, 'A' + i);
		IO.prints(window, ": ");

		switch(floppy[i])
		{
		case 1:
			IO.prints(window, "360kb 5.25in");
			break;
		case 2:
			IO.prints(window, "1.2mb 5.25in");
			break;
		case 3:
			IO.prints(window, "720kb 3.5in");
			break;
		case 4:
			IO.prints(window, "1.44mb 3.5in");
			break;
		case 5:
			IO.prints(window, "2.88mb 3.5in");
			break;
		default:
			IO.prints(window, "No floppy drive");
		}
	}

	IO.putc(window, '\n');
}
