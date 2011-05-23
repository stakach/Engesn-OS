#ifndef __Pconsole_h__
#define __Pconsole_h__

#include "keyboard.h"
#include "video.h"
#include "string.h"
#include "cmos.h"
#include "devices.h"
#include "Pkernel.h"
#include "kerndefs.h"
#include "speaker.h"



class Console : private DateTime, private Speaker
{
	public:
		Console() {major = 0; minor = 4; build = 11;};
		unsigned int start(int Window); /*Engesn temporary input console*/
		void setKBRate(int Window, unsigned char *rate);
		void dispTimeDate(int Window, unsigned char *display, unsigned char *type);
		int restartIt(int Window, unsigned char *p1, unsigned char *p2);

	private:
		void strsplit(unsigned char *str1, unsigned char *p1,unsigned char *p2,unsigned char *p3,unsigned char *p4,unsigned char *p5);

		unsigned short major;
		unsigned short minor;
		unsigned short build;
		char str[21];
		time_st_t sysTime; /*System start time*/
		date_st_t sysDate; /*System start date*/


		#define BUFFYSIZE 256 /*amount of characters acknowledged on the screen*/
};





#endif

