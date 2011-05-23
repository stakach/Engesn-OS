#ifndef __speaker_h__
#define __speaker_h__

#include "kerndefs.h"
#include "asmf.h"


/*The values for the different standard notes*/
/*-------------------------------------------*/
#define BOTTOMC	9121 /*Frequency 130.81*/
#define LOCS	8609
#define LOD		8126
#define LODS	7670
#define LOE		7239
#define LOF		6833
#define LOFS	6449
#define LOG		6087
#define LOGS	5746
#define LOA		5423
#define LOAS	5119
#define LOB		4831 /*Frequency 246.94*/
#define MIDC	4560 /*Key values for speaker, C, C# (CS), A, A#, ect.*/
#define HICS	4304
#define HID		4063
#define HIDS	3834
#define HIE		3619
#define HIF		3416
#define HIFS	3224
#define HIG		3043
#define HIGS	2873
#define HIA		2711
#define HIAS	2559
#define HIB		2415
#define UPPERC	2280 /*Frequency 523.25*/
#define UPPERCS 2152
#define UPPERD	2031
#define UPPERDS 1917
#define UPPERE	1809
#define UPPERF	1715
#define UPPERFS 1612
#define UPPERG	1521
#define UPPERGS 1436
#define UPPERA	1355
#define UPPERAS 1292
#define UPPERB	1207
#define TOPC	1140 /*Frequency 1046.50*/
/*-------------------------------------------*/

class Speaker
{
	public:
		void on(unsigned int pitch);
		void off();
		void beep(unsigned int delay, unsigned int pitch);
		void play(unsigned char *notes);
};


#endif
