#ifndef _Pkernel_H_
#define _Pkernel_H_

#include "time.h"
#include "kerndefs.h"
#include "speaker.h"
#include "A20.h"
#include "asmf.h"
#include "keyboard.h"
#include "video.h"
#include "dispIO.h"
#include "string.h"
#include "cmos.h"
#include "devices.h"
#include "vect.h"


/*Local Declarations*/
/********************/
void k_main();
void restart(int); /* 0 Shutdown, 1 Cold boot, Any other number Warm boot */
void assert(int Window, char expr, char *error_msg); /*error display*/
void Delay(unsigned int MilliSecs); /*Pauses execution for length of time*/



unsigned int startConsole(int Window); /*input temporary console*/


/*Global Variables*/
/******************/
extern unsigned int A20E; /*Memory size and A20 status*/
extern time_st_t sysTime;
extern date_st_t sysDate;

extern int StatusBar; /*global text windows*/



/*****************Old Stuff:*******************************/
/*char *ntostr(ulong n, ulong b, char *s); needs replacing*/
/*int detect_video_type(void); */


#endif
