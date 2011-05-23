#ifndef __speaker_h__
#define __speaker_h__

/*The values for the different standard notes*/
/*-------------------------------------------*/
#define BOTTOMC	131 /*Frequency 130.82*/
#define LOCS	139
#define LOD		147
#define LODS	156
#define LOE		165
#define LOF		175
#define LOFS	185
#define LOG		196
#define LOGS	208
#define LOA		220
#define LOAS	233
#define LOB		247 /*Frequency 246.94*/
#define MIDC	262 /*Key values for speaker, C, C# (CS), A, A#, ect.*/
#define HICS	277
#define HID		294
#define HIDS	311
#define HIE		330
#define HIF		349
#define HIFS	370
#define HIG		392
#define HIGS	415
#define HIA		440
#define HIAS	466
#define HIB		494
#define UPPERC	523 /*Frequency 523.25*/
#define UPPERCS 554
#define UPPERD	587
#define UPPERDS 622
#define UPPERE	660
#define UPPERF	696
#define UPPERFS 740
#define UPPERG	784
#define UPPERGS 831
#define UPPERA	881
#define UPPERAS 924
#define UPPERB	989
#define TOPC	1047 /*Frequency 1046.50*/
/*-------------------------------------------*/

class Speaker
{
	public:
		void on(unsigned int pitch);
		void off();
		void beep(unsigned int delay, unsigned int pitch);
};


#endif
