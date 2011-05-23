/*---------------------------------------------------------------------------------------------------
File Name : time.c      Purpose : Functions for obtaining system time
---------------------------------------------------------------------------------------------------*/

#include "cmos.h"
#include "time.h"

void get_time(time_st_t *time) /*gets the time from the cmos*/
{
	time -> second = cmos_read_bcd(SECOND);
	time -> minute = cmos_read_bcd(MINUTE);
	time -> hour   = cmos_read_bcd(HOUR);
}

void get_date(date_st_t *date) /*gets the date from the cmos*/
{
	date -> year  = (cmos_read_bcd(0x32) * 100) + cmos_read_bcd(YEAR); /*0x32 is the century number in the cmos*/
	date -> month = cmos_read_bcd(MONTH);
	date -> date  = cmos_read_bcd(DATE);
	date -> day   = cmos_read_bcd(DAY);
}

