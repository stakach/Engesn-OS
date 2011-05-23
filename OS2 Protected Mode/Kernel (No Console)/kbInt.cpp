#include "keyboard.h"


extern Keyboard keyboard;


static int kb_special(uchar key);

void keyboardInt();
void UpdateLeds(unsigned char led); /*Turns on[off] LEDs*/ 


#define BUFFSIZE 129


static char normal[] = {
  0x00,0x1B,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
'q','w','e','r','t','y','u','i','o','p','[',']',0x0D,0x80,
'a','s','d','f','g','h','j','k','l',';',047,0140,0x80,
0134,'z','x','c','v','b','n','m',',','.','/',0x80,
'*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,'0',0177
};

static char shifted[] = {
  0,033,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
'Q','W','E','R','T','Y','U','I','O','P','{','}',015,0x80,
'A','S','D','F','G','H','J','K','L',':',042,'~',0x80,
'|','Z','X','C','V','B','N','M','<','>','?',0x80,
'*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,'7','8','9',0x80,'4','5','6',0x80,
'1','2','3','0',177
};

static char capsNormal[] = {
  0x00,0x1B,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
'Q','W','E','R','T','Y','U','I','O','P','[',']',0x0D,0x80,
'A','S','D','F','G','H','J','K','L',';',047,0140,0x80,
'|','Z','X','C','V','B','N','M',',','.','/',0x80,
'*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,'0',0177
};

static char capsShifted[] = {
  0,033,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
'q','w','e','r','t','y','u','i','o','p','{','}',015,0x80,
'a','s','d','f','g','h','j','k','l',':',042,'~',0x80,
0134,'z','x','c','v','b','n','m','<','>','?',0x80,
'*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,'7','8','9',0x80,'4','5','6',0x80,
'1','2','3','0',177
};

char buffer[BUFFSIZE];
int  buffend = 0;

unsigned char scanCode;

Func_Key FKey[10] =
{
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

static bool shift = false;
static bool ctrl = false;
static bool alt = false;
static bool caps = false;
static bool num = false;

static unsigned char leds; /*The leds that are on and off on the keyboard*/


void keyboardInt()
{
	unsigned char asciiCode;

	disable();

	scanCode = inportb(0x60);
	outportb(0x20, 0x20);

	if(!(kb_special(scanCode) | (scanCode >= 0x80)))
	{
		if(shift)
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

		if(buffend != (BUFFSIZE - 1))
		{
			buffend++;
		}
		buffer[buffend] = asciiCode;

	}

	enable();

	asm("mov %ebp,%esp");
	asm("pop %ebp");
	asm("iret");
}


static int kb_special(uchar key)
{
	static bool specKeyUp = true;
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
				num = !num;
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

void UpdateLeds(unsigned char led) /*Updates the keyboard leds*/
{
	if(led == 0)
	{
		leds = 0;
	}
	else
	{
		if (leds == (leds|led))
		{
			leds = leds^led;
		}
		else
		{
			leds = leds | led;
		}
	}
	keyboard.SendData(0xED);
	keyboard.SendData(leds);
}

