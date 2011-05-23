
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\include\speaker.h"
#include "..\include\asmf.h"		//outportb, inportb
#include "..\include\PIT.h"			//pit

void Speaker::on(unsigned int pitch)	//Enables the speaker and sets frequency
{
	pit.init(pitch, 2);
	outportb(0x61, (inportb(0x61) | 3));
}

void Speaker::off()						//Disables the speaker
{
	outportb(0x61, (inportb(0x61) & 0xfc));
}

void Speaker::beep(unsigned int Delay, unsigned int pitch)	//turns on the speaker for a defined period of time
{
	on(pitch);
	delay(Delay);
	off();
}

