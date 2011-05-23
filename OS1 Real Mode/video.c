

#include "video.h"
#include "kerndefs.h"

boolean screenEnabled = TRUE;

unsigned short crtc_mem;


void vidSetup() /*sets up the variables for direct vid access in current mode*/
{
	char c = (*(ushort*)0x410&0x30);/*Detects video card type, vga or mono*/

	/* C can be 0x00 or 0x20 for colour, 0x30 for mono*/
	if(c == 0x30)
	{
		screen_mem = (int far *)0xb0000000;
		crtc_mem   = 0x3B4;	/* mono*/
	}
	else
	{
		screen_mem = (int far *)0xb8000000;
		crtc_mem   = 0x3D4;	/* colour*/
	}
}

void screenStatus(boolean Enabled)
{
	if(Enabled == TRUE)
	{
		outportw(0x3c4, 1); /*Turns monitor on*/
		outportw(0x3c5, 0x00);
	}
	else
	{
		outportw(0x3c4, 1); /*Turns monitor off*/
		outportw(0x3c5, inportb(0x3c5) | 0x20);
	}
	screenEnabled = Enabled;
}
