
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\include\cmos.h"
#include "..\include\asmf.h"	//outportb, inportb
#include "..\include\maths.h"	//bcd2dec

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

int Cmos::read(unsigned char address)		//reads a byte out of the cmos
{
	while(busy());
	outportb(CMOS_OUT_PORT, 0x80|address);	//Use 0x80 coz we need to disable NMI
	return inportb(CMOS_IN_PORT);
}

bool Cmos::busy()							//checks if the cmos is ready
{
	outportb(CMOS_OUT_PORT, 0x80|STATUS_A);
	if(inportb(CMOS_IN_PORT) & 0x80)		//Bit 7 is set in Status Register A if Clock is busy
		return true;
	else
		return false;
}

void Cmos::time(time_stamp &time)			//gets the time from the cmos
{
	time.second = bcd2dec(read(SECOND));
	time.minute = bcd2dec(read(MINUTE));
	time.hour   = bcd2dec(read(HOUR));
}

void Cmos::date(date_stamp &date)			//gets the date from the cmos
{
	date.year  = (bcd2dec(read(0x32)) * 100) + bcd2dec(read(YEAR));	//0x32 is the century number in the cmos
	date.month = bcd2dec(read(MONTH));
	date.date  = bcd2dec(read(DATE));
	date.day   = bcd2dec(read(DAY));
}

