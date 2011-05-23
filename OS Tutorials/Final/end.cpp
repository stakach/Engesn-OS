
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "include\end.h"
#include "include\IStream.h"	//cin
#include "include\asmf.h"		//disable, halt

	#ifdef __cplusplus
		extern "C" 
		{
	#endif 

			void _atexit();

	#ifdef __cplusplus
		}
	#endif

using std::cin;

void kshutdown(int Type)		//the different types of shutdown
{
	unsigned short *BiosMem = (unsigned short *)0x472;

	_atexit();					// Call global de-constructors

	disable();					// disable interrupts

	cin.FlushBuffer();			// clear keyboard buffer
	
	if(Type)					// pulse the CPU reset line (if not 0 -> power off)
	{
		switch (Type)				// 0 shutdown, 1 Cold boot, (Other) Warm boot
		{
		case 1:
				*BiosMem = 0x0000;	// Cold boot
				break;
		default:
				*BiosMem = 0x1234;	// Warm boot
		}
		cin.SendCommand(0xFE);		//Pulse reset-line low using the keyboard
	}

	while(1)
	{
		halt();					// Halt the cpu
	}
}

/*
+Values for BiosMem+
--------------------
	0x0000 - Cold boot } ____| Only these options
	0x1234 - Warm boot }     | are used.
	0x4321 - PS/2
	0x5678 - Suspend system
	0x9abc - Manufacturing test mode
	0xabcd - POST loop mode
	0x0064 - Burn-in mode
*/

