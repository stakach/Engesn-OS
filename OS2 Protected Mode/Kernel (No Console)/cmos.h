/*--------------------------------------------------------------------------------------------------
File Name : cmos.h      Purpose : Declarations for accessing CMOS
--------------------------------------------------------------------------------------------------*/

#ifndef __cmos_h__
#define __cmos_h__

#include "kerndefs.h"
#include "asmf.h"

class Cmos
{
	public:
		int read(byte);
		int read_bcd(byte address);
		

	private:
		bool busy();
		int bcd2bin(byte);

		#define CMOS_OUT_PORT 0x70
		#define CMOS_IN_PORT 0x71

		#define SECOND 0x00
		#define MINUTE 0x02
		#define HOUR 0x04
		#define DAY 0x06
		#define DATE 0x07
		#define MONTH 0x08
		#define YEAR 0x09
		#define STATUS_A 0x0A
		#define STATUS_B 0x0B
		#define STATUS_C 0x0C
		#define STATUS_D 0x0D
};

#endif

