/*--------------------------------------------------------------------------------------------------
File Name : cmos.h      Purpose : Declarations for accessing CMOS
--------------------------------------------------------------------------------------------------*/

#ifndef __cmos_h__
#define __cmos_h__

typedef struct time_str_tag
{
	int hour;
	int minute;
	int second;
}time_stamp;

typedef struct date_str_tag
{
	int year;
	int month;
	int date;
	int day;
}date_stamp;

class Cmos
{
	public:
		int read(unsigned char);
		void time(time_stamp &);
		void date(date_stamp &);

	private:
		bool busy();
};

#endif

