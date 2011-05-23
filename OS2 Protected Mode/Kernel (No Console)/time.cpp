/*---------------------------------------------------------------------------------------------------
File Name : time.c      Purpose : Functions for obtaining system time
---------------------------------------------------------------------------------------------------*/

#include "time.h"

void DateTime::time(time_st_t &time) /*gets the time from the cmos*/
{
	time.second = read_bcd(SECOND);
	time.minute = read_bcd(MINUTE);
	time.hour   = read_bcd(HOUR);
}

void DateTime::date(date_st_t &date) /*gets the date from the cmos*/
{
	date.year  = (read_bcd(0x32) * 100) + read_bcd(YEAR); /*0x32 is the century number in the cmos*/
	date.month = read_bcd(MONTH);
	date.date  = read_bcd(DATE);
	date.day   = read_bcd(DAY);
}

