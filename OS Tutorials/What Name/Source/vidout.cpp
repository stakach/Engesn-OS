/*++

Copyright (c) 2003-2004  Engesn OS Derivative

Module Name:

	Video.cpp

Abstract:

	Contains routines for debugging output.

Author:

	Stephen von Takach 27-April-2004

Revision History:

--*/


#include "..\inc\def.h"
#include "..\i386\inc\i386.h"
#include "..\i386\inc\asmf.h"
#include "inc\Video.h"


Video Console;


Video::Video()
{


	//
	// Detects video card type, vga or mono.
	//

	char c = (*(unsigned short*)0x410&0x30);

	//
	// c can be 0x00 or 0x20 for Colour, 0x30 for mono.
	//

	if(c == 0x30)
	{
		VideoMem = (unsigned short*) 0xb0000;
		CrtcMem = 0x3B4;	// mono.
	}
	else
	{
		VideoMem = (unsigned short*) 0xb8000;
		CrtcMem = 0x3D4;	// Colour.
	}

	Colour = 0x17;
	Text = 0x7;
	Back = 0x1;

	ScrHeight = 25;
	ScrWidth = 80;


	//
	// Clear sets XPos and YPos.
	//

	Clear();
}


Video::~Video() {}


void Video::Clear()
{


	//
	// Sets all video memory to display ' ' (blank).
	//

	unsigned int i;

	for(i = 0; i < (ScrWidth * ScrHeight); i++)
	{
		VideoMem[i] = (unsigned char) ' ' | (Colour << 8) ;
	}

	GotoXY(0, 0);
}


void Video::Write(char *cp)
{

	//
	// Puts every char in a string onto the screen.
	//

	while((*cp) != 0) Put(*(cp++));
}


void Video::Put(char c)
{
	int t = XPos + YPos * ScrWidth;		// get linear address
	int Tab;

	switch(c)
	{
	case '\n':							// -> newline (with implicit cr)
		YPos++;

	case '\r':							// -> carriage return
		XPos = 0;
		break;

	case '\t':							// -> Calculate Tab position
		Tab = (t % ScrWidth) % 8;
		t += 16 - Tab;
		XPos += (8 - Tab);
		break;

	case 8:								// -> backspace 
		if(t > 0) t--;
										// if not in home position, step Back 
		if(XPos > 0)
		{
			XPos--;
		}
		else if(YPos > 0)
		{
			YPos--;
			XPos = ScrWidth - 1;
		}
										// Put space under the cursor
		*(VideoMem + t) = ' ' | (Colour << 8); 
		break;

	default:							// -> all other CHARacters 
		if(c < ' ') break;				// ignore non prINTable ascii CHARs 
		*(VideoMem + t) = c | (Colour << 8);
		XPos++;							// step cursor one CHARacter 
		if(XPos == ScrWidth)			// to next line if required
		{
			XPos = 0;
			YPos++;
		}
		break;
	}

	if(YPos == ScrHeight)				// the cursor moved off of the screen?
	{
		ScrollUp();						// scroll the screen up
		YPos--;							// and move the cursor Back
	}
										// and finally, set the cursor
	SetCursor();
}


void Video::ScrollUp()
{


	//
	// scroll the screen up one line.
	//

	int t;

	for(t = 0; t < ScrWidth * (ScrHeight - 1); t++)		// scroll every line up
		*(VideoMem + t) = *(VideoMem + t + ScrWidth);
	for(; t < ScrWidth * ScrHeight; t++)				//Clear the bottom line
		*(VideoMem + t) = ' ' | (Colour << 8);

}


void Video::SetCursor()
{


	//
	// Hardware move cursor.
	//

    unsigned short Offset;

	Offset = XPos + YPos * ScrWidth;	// 80 CHARacters per line
	outportb(CrtcMem, 14);				// MSB of Offset to CRTC reg 14
	outportb(CrtcMem + 1, Offset >> 8);
	outportb(CrtcMem, 15);				// LSB of Offset to CRTC reg 15
	outportb(CrtcMem + 1, Offset);
}


void Video::GotoXY(unsigned int x, unsigned int y)
{

	//
	// Software move cursor.
	//

	XPos = x; YPos = y;			// set cursor to location
	SetCursor();				// call the hw cursor setup
}


void Video::SetTextColour(COLOURS Col)
{

	//
	// Sets the Colour of the Text being displayed.
	//

	Text = (unsigned char)Col;
	Colour = (Back << 4) | Text;
}


void Video::SetBackColour(COLOURS Col)
{

	//
	// Sets the Colour of the background being displayed.
	//

	if(Col > 7)
	{
		Col = BLACK;
	}
	Back = (unsigned char)Col;
	Colour = (Back << 4) | Text;
}


void Video::SetColour(COLOURS Text, COLOURS Back, BOOLEAN Blink)
{

	//
	// Sets the Colour of the Text being displayed.
	//

	SetTextColour(Text);
	SetBackColour(Back);
	if(Blink)
	{
		Colour = (Back << 4) | Text | 128;
	}
}


void Video::CursorVisible(BOOLEAN Status)
{

	//
	// Displays or hides the Text cursor.
	//

	if(Status)
	{
		outportb(CrtcMem, 0x0A);
		outportb(CrtcMem + 1, (inportb(CrtcMem + 1) & 0x1F));
		SetCursor();
	}
	else
	{
		outportb(CrtcMem, 0x0A);
		outportb(CrtcMem + 1, (inportb(CrtcMem + 1) | 0x20));
	}
}


static unsigned short strlen(char *s)
{
	unsigned short len = 0;

	while(*s != '\0')
	{
		*s++;
		len++;
	}

	return len;
}


static unsigned char s[22];
static char * itoa(ULONG n, unsigned char base)
{
	ULONG i, j;
	unsigned char tmp[22], digit;

	i = 0;
	do
	{
		j = n;
		n /= base;		// divide num by base
		n *= base;		// multiply num by b to isolate the digit
		digit = j - n;
		
						// change the numerical digit to printable ASCII value
		if(digit <= 9)
		{
			tmp[i] = digit + '0';
		}
		else
		{
			tmp[i] = digit - 0x0A + 'A';
		};
		n /= base;		// divide num by b again to get to the next digit
		i++;
	} while(n != 0);

	if(base == 16)
	{
		tmp[i] = 'x';
		i++;
		tmp[i] = '0';
		i++;
	}
	
	tmp[i] = '\0';		// add the terminator
	
						// reverse string
	j = strlen((char *)tmp) - 1;
	for (i = 0; i < strlen((char *)tmp); i++) 
	{
		s[i] = tmp[j];
		j--;
	}
	s[i] = '\0';

	return (char *)s;
}


void Video::Write(ULONG Num, unsigned int Base)
{
	Write(itoa(Num, Base));
}
