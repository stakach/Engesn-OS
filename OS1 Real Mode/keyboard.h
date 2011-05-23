#ifndef _keyboard_H_
#define _keyboard_H_

#include "kerndefs.h"


/*Led Numbers*/
#define ScrollLock (unsigned char)0x01
#define NumLock (unsigned char)0x02
#define CapsLock (unsigned char)0x04

/*typeamatic rate and delay values*/
/*													Value Rate    Value Rate
     This chart is a partial guide for the repeat    0  = 30.0    0Ah = 10.0
     rate (bits 0-4).  You can interpolate for       1  = 26.7    0Dh =  9.2
     values not shown, but let's face it, you're     2  = 24.0    10h =  7.5
     only interested in the fastest rates.           4  = 20.0    14h =  5.0
                                                     8  = 15.0    1Fh =  2.0
  Delay values:
  00=250ms; 01=500ms; 10=750ms; 11=1000ms
*/
#define KDefault 0x02 /*Default Delay-Rate*/

#define K25Delay 0x00 /*Delay values (25 fastest -> 01 Slowest)*/
#define K50Delay 0x20
#define K75Delay 0x40
#define K01Delay 0x60

#define K0Rate 0x0 /*Rate values (0 fastest -> L4 Slowest)*/
#define K1Rate 0x1
#define K2Rate 0x2
#define K4Rate 0x4
#define K8Rate 0x8
#define KL0Rate 0x0A
#define KL1Rate 0x0D
#define KL2Rate 0x10
#define KL3Rate 0x14
#define KL4Rate 0x1F


/* Scancodes for non-standard keyboard keys */ 
/* ---------------------------------------- */
#define UP		0x4800
#define DOWN	0x5000
#define LEFT	0x4B00
#define RIGHT	0x4D00
#define PGUP	0x4900
#define CTLPGUP	0x8400
#define PGDN	0x5100
#define CTLPGDN	0x7600
#define HOME	0x4700
#define END	0x4f00
#define ESC	0x011b

#define F1	0x3B00
#define F2	0x3C00
#define F3	0x3D00
#define F4	0x3E00
#define F5	0x3F00
#define F6	0x4000
#define F7	0x4100
#define F8	0x4200
#define F9	0x4300
#define F10	0x4400
#define F11     0x8500
#define F12     0x8600

#define CTRLF1      0x5E00
#define CTRLF2      0x5F00
#define CTRLF3      0x6000
#define CTRLF4      0x6100
#define CTRLF5      0x6200
#define CTRLF6      0x6300
#define CTRLF7      0x6400
#define CTRLF8      0x6500
#define CTRLF9      0x6600
#define CTRLF10     0x6700
#define CTRLF11     0x8900
#define CTRLF12     0x8A00

#define ALTF1      0x6800
#define ALTF2      0x6900
#define ALTF3      0x6A00
#define ALTF4      0x6B00
#define ALTF5      0x6C00
#define ALTF6      0x6D00
#define ALTF7      0x6E00
#define ALTF8      0x6F00
#define ALTF9      0x7000
#define ALTF10     0x7100
#define ALTF11     0x8B00
#define ALTF12     0x8C00

#define ALTA    0x1E00
#define ALTB	0x3000
#define ALTC	0x2E00
#define ALTD	0x2000
#define ALTE	0x1200
#define ALTF	0x2100
#define ALTG	0x2200
#define ALTH    0x2300
#define ALTI    0x1700
#define ALTJ    0x2400
#define ALTK    0x2500
#define ALTL    0x2600
#define ALTM    0x3200
#define ALTN    0x3100
#define ALTO    0x1800
#define ALTP	0x1900
#define ALTQ	0x1000
#define ALTR    0x1300
#define ALTS	0x1F00
#define ALTT    0x1400
#define ALTU    0x1600
#define ALTV    0x2F00
#define ALTW	0x1100
#define ALTX    0x2D00
#define ALTY    0x1500
#define ALTZ    0x2C00

#define CTRLA    0x1E01
#define CTRLB    0x3002
#define CTRLC    0x2E03
#define CTRLD    0x2004
#define CTRLE    0x1205
#define CTRLF    0x2106
#define CTRLG    0x2207
#define CTRLH    0x2308
#define CTRLI    0x1709
#define CTRLJ    0x240A
#define CTRLK    0x250B
#define CTRLL    0x260C
#define CTRLM    0x320D
#define CTRLN    0x310E
#define CTRLO    0x180F
#define CTRLP    0x1910
#define CTRLQ    0x1011
#define CTRLR    0x1312
#define CTRLS    0x1F13
#define CTRLT    0x1414
#define CTRLU    0x1615
#define CTRLV    0x2F16
#define CTRLW    0x1117
#define CTRLX    0x2D18
#define CTRLY    0x1519
#define CTRLZ    0x2C1A
/* ---------------------------------------- */


void waitKey();		  /*Detects a keypress without bios*/
char getch();		/*Waits for a key without pausing the computer (allows for MT)*/

void kbFlushBuffer(); /*flushes keyboard buffer*/
void kbWaitForReady(void); /*Waits for keyboard controller to accept input*/

void kbSendCommand(unsigned char c); /*send commands to keyboard port 0x64*/
void kbSendData(unsigned char data); /*send commands and data to port 0x60*/

void kbUpdateLeds(void); /*Turns on LEDs*/ 
void kbSetRate(unsigned char rate); /*Sets typeamatic rate and delay*/

void kbSetup(); /*Sets up the keyboard for the OS*/


extern unsigned char leds; /*The leds that are on and off on the keyboard*/


#endif
