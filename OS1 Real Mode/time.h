/*--------------------------------------------------------------------------------------------------
File Name : time.h      Purpose : Declarations for obtaining system time
--------------------------------------------------------------------------------------------------*/

#ifndef __time_h__
#define __time_h__

typedef struct time_str_tag
{
	int hour;
	int minute;
	int second;
}time_st_t;

typedef struct date_str_tag
{
	int year;
	int month;
	int date;
	int day;
}date_st_t;

void get_time(time_st_t*);
void get_date(date_st_t*);

#endif

