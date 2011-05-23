
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\include\PIT.h"
#include "..\include\asmf.h"		//outportb
#include "..\include\Interrupt.h"	//intcontrol
#include "..\include\OStream.h"		//cout
#include "..\include\semaphore.h"	//semStart, semEnd

using std::cout;


#define MILLISEC 10				// Take a 10ms fixed timer rate
#define FREQ (1000/MILLISEC)	// Timer freqency for MILLISEC
#define PRECISION 8				// Calibration precision

#define	CONTROL_PORT	0x43	// I/O for control
#define	COUNTER_0_PORT	0x40
#define	COUNTER_2_PORT	0x42


PIT pit;

namespace
{
	unsigned long delay_count = 1;
	volatile unsigned long ticks = 0;

	volatile void __delay(unsigned long loops)
	{
		unsigned long c;
		for (c = 0; c < loops; c++);
	}

	void delayCalibInt()
	{							//The timer ISR
		ticks++;				//Increment the ticks counter
		outportb(0x20, 0x20);	//tell pic we have acknowledged the interrupt
	/*
		asm volatile ("mov %ebp,%esp");
		asm volatile ("pop %ebp");	//Redundant due to leave
	*/
		asm volatile ("leave");
		asm volatile ("iret");	//return to executing code
	}

	volatile unsigned long calibrateDelayLoop()	//The Linux method
	{
		unsigned int prevtick;			// Temporary variable
		unsigned int i;					// Counter variable
		unsigned int calib_bit;			// Bit to calibrate (see below)
		

		pit.init(FREQ, 0);				// Initialise timer interrupt with MILLISECOND ms interval
		intcontrol.enable_irq(0, delayCalibInt, INT_GATE|BITS_32|PRESENT|RING_2);


		// Stage 1:	Coarse calibration	:

		do 
		{
			delay_count <<= 1;	// Next delay count to try

			prevtick = ticks;	// Wait for the start of the next timer tick
			while (prevtick == ticks);

			prevtick = ticks;	// Start measurement now
			__delay(delay_count);		// Do the delay until delay is just too big
		} while (prevtick == ticks);

		delay_count >>= 1;		// Get bottom value for delay

		// Stage 2:	Fine calibration	:

		calib_bit = delay_count;	// Which bit are we going to test

		for (i = 0; i < PRECISION; i++) 
		{
			calib_bit >>= 1;		// Next bit to calibrate
			if (!calib_bit)
				break;				// If we have done all bits, stop
	
			delay_count |= calib_bit;	// Set the bit in delay_count

			prevtick = ticks;		// Wait for the start of the next timer tick
			while (prevtick == ticks);

			prevtick = ticks;		// Start measurement now
			__delay(delay_count);	// Do the delay

			if (prevtick != ticks)	// If a tick has passed, turn the calibrated bit back off
			{
				delay_count &= ~calib_bit;
			}
		}

		// We're finished:	Do the finishing touches:

		intcontrol.disable_irq(0);	// Our PIT can stop now

		delay_count /= MILLISEC;	// Calculate delay_count for 1ms
		return delay_count;
	}
}

PIT::PIT()
{
	cout << "\n  Calibrating delay loop...";

	calibrateDelayLoop();

	cout << "\t\t\t\t[ DONE ]\n";
}

void PIT::init(long h, unsigned char channel)	//Sets the frequency of a PIT channel.
{
	unsigned int temp = 0;

	temp = 1193182 / h;

	semStart();

	outportb(CONTROL_PORT, (channel * 0x40) + 0x30 + 0x6);
	outportb((0x40 + channel), (unsigned char) temp);
	outportb((0x40 + channel), (unsigned char) (temp >> 8));

	semEnd();
}

void delay(unsigned long milliseconds)
{
	__delay(milliseconds * delay_count);	// Delay milliseconds ms
}


