#ifndef _Pkernel_H_
#define _Pkernel_H_


#include "asmf.h"
#include "devices.h"
#include "kerndefs.h"
#include "keyboard.h"
#include "speaker.h"
#include "string.h"
#include "time.h"
#include "Video.h"
#include "cmos.h"
#include "interrupt.h"
#include "PConsole.h"


class System : private DateTime
{
public:
	System();
	void main();
	void restart(int Type);
	void assert(int Window, char expr, char *error_msg);

	time_st_t sysTime; /*System start time*/
	date_st_t sysDate; /*System start date*/
	int StatusBar; /*The token for the main text window*/
	int WinMain;

private:


};


	#ifdef __cplusplus
	extern "C" {
	#endif 

		void _atexit();

	#ifdef __cplusplus
	}
	#endif 


#endif

