
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\..\include\IStream.h"
#include "..\..\include\asmf.h"		//disable, enable, outportb, inportb


void keyboardInt();					//Keyboard interrupt
void UpdateLeds(unsigned char led);	//Turns on[off] LEDs

using std::cin;

Func_Key FKey[10] =		//A table for assigning functions to the function keys
{						// this is done through the keyboard driver
        {false, 0},
        {false, 0},
        {false, 0},
        {false, 0},
        {false, 0},
        {false, 0},
        {false, 0},
        {false, 0},
        {false, 0},
        {false, 0}
};

namespace 
{
	int kb_special(unsigned char key);	//Detects non-ascii key presses 

	const unsigned int KEYBUFFSIZE = 129;	//Size of our key buffer

	char normal[] = {					//Keyboard character maps (Look up table)
	  0x00,0x1B,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
	'q','w','e','r','t','y','u','i','o','p','[',']',0x0D,0x80,
	'a','s','d','f','g','h','j','k','l',';',047,0140,0x80,
	0134,'z','x','c','v','b','n','m',',','.','/',0x80,
	'*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,10,0x80,0x80,0x80,0x80,0x80,'-',0x80,0x80,0x80,'+',
	0x80,0x80,0x80,0x80,0177
	};

	char shifted[] = {
	  0,033,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
	'Q','W','E','R','T','Y','U','I','O','P','{','}',015,0x80,
	'A','S','D','F','G','H','J','K','L',':',042,'~',0x80,
	'|','Z','X','C','V','B','N','M','<','>','?',0x80,
	'*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,10,0x80,0x80,'7','8','9','-','4','5','6','+',
	'1','2','3','0',177
	};

	char capsNormal[] = {
	  0x00,0x1B,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
	'Q','W','E','R','T','Y','U','I','O','P','[',']',0x0D,0x80,
	'A','S','D','F','G','H','J','K','L',';',047,0140,0x80,
	'|','Z','X','C','V','B','N','M',',','.','/',0x80,
	'*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,10,0x80,0x80,0x80,0x80,0x80,'-',0x80,0x80,0x80,'+',
	0x80,0x80,0x80,0x80,0177
	};

	char capsShifted[] = {
	  0,033,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
	'q','w','e','r','t','y','u','i','o','p','{','}',015,0x80,
	'a','s','d','f','g','h','j','k','l',':',042,'~',0x80,
	0134,'z','x','c','v','b','n','m','<','>','?',0x80,
	'*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,10,0x80,0x80,'7','8','9','-','4','5','6','+',
	'1','2','3','0',177
	};

	bool shift = false;	//Key status
	bool ctrl = false;
	bool alt = false;
	bool caps = false;
	bool num = false;
	bool scroll = false;

	unsigned char leds; //The led mask


	int kb_special(unsigned char key)
	{
		static bool specKeyUp = true;	//Is a key already been or being presses
		switch(key) 
		{
			case 0x36: //R-Shift down
			case 0x2A: //L-Shift down
				shift = true;
				break;
			case 0xB6: //R-Shift up
			case 0xAA: //L-Shift up
				shift = false;
				break;
			case 0x1D: //Control down
				ctrl = true;
				break;
			case 0x9D: //Control up
				ctrl = false;
				break;
			case 0x38: //Alt down
				alt = true;
				break;
			case 0xB8: //Alt up
				alt = false;
				break;
			case 0x3A: //Caps down
				if(specKeyUp == true) 
				{
					caps = !caps;
					UpdateLeds(CapsLock);
					specKeyUp = false;
				}
				break;
			case 0x45: //Num down
				if(specKeyUp == true)
				{
					num = !num;
					UpdateLeds(NumLock);
					specKeyUp = false;
				}
				break;
			case 0x46: //Scroll down
				if(specKeyUp == true)
				{
					scroll = !scroll;
					UpdateLeds(ScrollLock);
					specKeyUp = false;
				}
				break;
			case 0x3B: //F1 Down
				if(specKeyUp && FKey[0].enabled)
				{
					FKey[0].func();
					specKeyUp = false;
				}
				break;
			case 0x3C: //F2 Down
				if(specKeyUp && FKey[1].enabled)
				{
					FKey[1].func();
					specKeyUp = false;
				}
				break;
			case 0x3D: //F3 Down
				if(specKeyUp && FKey[2].enabled)
				{
					FKey[2].func();
					specKeyUp = false;
				}
				break;
			case 0x3E: //F4 Down
				if(specKeyUp && FKey[3].enabled)
				{
					FKey[3].func();
					specKeyUp = false;
				}
				break;
			case 0x3F: //F5 Down
				if(specKeyUp && FKey[4].enabled)
				{
					FKey[4].func();
					specKeyUp = false;
				}
				break;
			case 0x40: //F6 Down
				if(specKeyUp && FKey[5].enabled)
				{
					FKey[5].func();
					specKeyUp = false;
				}
				break;
			case 0x41: //F7 Down
				if(specKeyUp && FKey[6].enabled)
				{
					FKey[6].func();
					specKeyUp = false;
				}
				break;
			case 0x42: //F8 Down
				if(specKeyUp && FKey[7].enabled)
				{
					FKey[7].func();
					specKeyUp = false;
				}
				break;
			case 0x43: //F9 Down
				if(specKeyUp && FKey[8].enabled)
				{
					FKey[8].func();
					specKeyUp = false;
				}
				break;
			case 0x44: //F10 Down
				if(specKeyUp && FKey[9].enabled)
				{
					FKey[9].func();
					specKeyUp = false;
				}
				break;
			case 0xBA: //Caps Up
			case 0xBB: //F1 Up
			case 0xBC: //F2 Up
			case 0xBD: //F3 Up
			case 0xBE: //F4 Up
			case 0xBF: //F5 Up
			case 0xC0: //F6 Up
			case 0xC1: //F7 Up
			case 0xC2: //F8 Up
			case 0xC3: //F9 Up
			case 0xC4: //F10 Up
			case 0xC5: //Num Up
			case 0xC6: //Scroll Up
				specKeyUp = true;
				break;
			case 0xE0:
				break;
			default:
				return(0);
		}
		return (1);
	}
} //end of namespace. We use the unnamed namespace in cpp instead of 'static'


char buffer[KEYBUFFSIZE];	//Our key buffer. Global so that our keyboard driver can access it
int  buffend = 0;			//Last key in the buffer for the driver
unsigned char scanCode;		//Last scan code recieved from the keyboard


void keyboardInt()		//Keyboard interrupt
{
	unsigned char asciiCode = 0;

	disable();			//Don't let interrupts bother us while handling one.

	scanCode = inportb(0x60);	//retrieve scan code

	if(!(kb_special(scanCode) | (scanCode >= 0x80)))
	{
		if(shift)		//Determine key that has been pressed
		{
			if(!caps)
			{
				asciiCode = shifted[scanCode];
			}
			else
			{
				asciiCode = capsShifted[scanCode];
			}
		}
		else
		{
			if(!caps)
			{
				asciiCode = normal[scanCode];
			}
			else
			{
				asciiCode = capsNormal[scanCode];
			}
		}

		if(num && (0x47 <= scanCode <= 0x53))
		{
			switch(scanCode)
			{
				case 0x47: asciiCode = '7'; break;
				case 0x48: asciiCode = '8'; break;
				case 0x49: asciiCode = '9'; break;
				case 0x4B: asciiCode = '4'; break;
				case 0x4C: asciiCode = '5'; break;
				case 0x4D: asciiCode = '6'; break;
				case 0x4F: asciiCode = '1'; break;
				case 0x50: asciiCode = '2'; break;
				case 0x51: asciiCode = '3'; break;
				case 0x52: asciiCode = '0'; break;
				case 0x53: asciiCode = '.'; break;
			}
		}

		if(buffend != (KEYBUFFSIZE - 1))
		{
			buffend++;
		}
		buffer[buffend] = asciiCode;

	}

	outportb(0x20, 0x20);		//tell pic we have acknowledged the interrupt
	enable();					//Enable interrupts

/*	
	asm volatile ("mov %ebp,%esp");
	asm volatile ("pop %ebp");	//Redundant due to leave
*/
	asm volatile ("leave");
	asm volatile ("iret");		//return to executing code
}

void UpdateLeds(unsigned char led)	//Updates the keyboard leds
{
	if(led == 0)
	{
		leds = 0;
	}
	else
	{
		if (leds == (leds|led))	//If led is already on
		{
			leds = leds^led;	// turn led off
		}
		else
		{
			leds = leds | led;	// else turn led on
		}
	}
	cin.SendData(0xED);
	cin.SendData(leds);			//update led status
}


