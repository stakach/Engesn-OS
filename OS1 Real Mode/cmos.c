

#include "kerndefs.h"
#include "cmos.h"

int cmos_read(byte address) /*reads a byte out of the cmos*/
{
	while(cmos_busy());
	outportb(CMOS_OUT_PORT, 0x80|address);		/*Use 0x80 coz we need to disable NMI*/
	return inportb(CMOS_IN_PORT);
}

int cmos_read_bcd(byte address)
{
	return bcd2bin(cmos_read(address));
}

boolean cmos_busy() /*checks if the cmos is ready*/
{
	outportb(CMOS_OUT_PORT, 0x80|STATUS_A);
	if(inportb(CMOS_IN_PORT) & 0x80)			/*Bit 7 is set in Status Register A if Clock is busy*/
		return TRUE;
	else
		return FALSE;
}

int bcd2bin(byte bcd) /*binary coded decimal to binary converter*/
{
	return((bcd & 0xF) + ((bcd >> 4) * 10));
}

/*BCD to BIN Math
  Its easy to explain with an example
  Input : bin = 25 BCD
		  bin = 0010 0101
		  bin & 0xF = 0000 0101          -> (a)
		  bin >> 4 = 0000 0010
		  (bin >> 4) * 10 = 0001 0100    -> (b)
		  (a) + (b) = 0001 1001 -> This is 25 in BINARY*/

