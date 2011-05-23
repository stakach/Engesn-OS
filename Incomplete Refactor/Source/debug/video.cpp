/*++

Copyright (c) 2003-2004  Engesn OS Derivative

Module Name:

	video.cpp

Abstract:

	Contains routines for debugging output.

Author:

	Stephen von Takach 27-April-2004

Revision History:

	* Stephen von Takach 5-July-2006
		- Updated for use with MeepOS.

--*/


#include <def.h>
#include <arch/asmf.h>			// outport
#include <debug/video.h>


using namespace DEBUG;


VIDOUT VIDOUT::Vidout;


VIDOUT::VIDOUT()
{


	//
	// Detects video card type, vga or mono.
	//

	CHAR c = (*(USHORT*)0x410&0x30);

	//
	// c can be 0x00 or 0x20 for Colour, 0x30 for mono.
	//

	if(c == 0x30)
	{
		VideoMem = (USHORT*) 0xb0000;
		CrtcMem = 0x3B4;	// mono.
	}
	else
	{
		VideoMem = (USHORT*) 0xb8000;
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


VOID VIDOUT::Clear()
{


	//
	// Sets all video memory to display ' ' (blank).
	//

	UINT i;

	for(i = 0; i < (ScrWidth * ScrHeight); i++)
	{
		VideoMem[i] = (UCHAR) ' ' | (Colour << 8) ;
	}

	GotoXY(0, 0);
}


VOID VIDOUT::Write(CHAR *cp)
{

	//
	// Puts every CHAR in a string onto the screen.
	//

	while((*cp) != 0) Put(*(cp++));
}


VOID VIDOUT::Put(const CHAR &c)
{
	INT t = XPos + YPos * ScrWidth;		// get linear address
	INT Tab;

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


VOID VIDOUT::ScrollUp()
{


	//
	// scroll the screen up one line.
	//

	INT t;

	for(t = 0; t < ScrWidth * (ScrHeight - 1); t++)		// scroll every line up
		*(VideoMem + t) = *(VideoMem + t + ScrWidth);
	for(; t < ScrWidth * ScrHeight; t++)				//Clear the bottom line
		*(VideoMem + t) = ' ' | (Colour << 8);

}


VOID VIDOUT::SetCursor()
{


	//
	// Hardware move cursor.
	//

    USHORT Offset;

	Offset = XPos + YPos * ScrWidth;	// 80 CHARacters per line
	outportb(CrtcMem, 14);				// MSB of Offset to CRTC reg 14
	outportb(CrtcMem + 1, Offset >> 8);
	outportb(CrtcMem, 15);				// LSB of Offset to CRTC reg 15
	outportb(CrtcMem + 1, Offset);
}


VOID VIDOUT::GotoXY(UINT x, UINT y)
{

	//
	// Software move cursor.
	//

	XPos = x; YPos = y;			// set cursor to location
	SetCursor();				// call the hw cursor setup
}


VOID VIDOUT::SetTextColour(COLOURS Col)
{

	//
	// Sets the Colour of the Text being displayed.
	//

	Text = (UCHAR)Col;
	Colour = (Back << 4) | Text;
}


VOID VIDOUT::SetBackColour(COLOURS Col)
{

	//
	// Sets the Colour of the background being displayed.
	//

	if(Col > 7)
	{
		Col = BLACK;
	}
	Back = (UCHAR)Col;
	Colour = (Back << 4) | Text;
}


VOID VIDOUT::SetColour(COLOURS Text, COLOURS Back, BOOLEAN Blink)
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


VOID VIDOUT::CursorVisible(BOOLEAN Status)
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


static USHORT strlen(CHAR *s)
{
	USHORT len = 0;

	while(*s != '\0')
	{
		*s++;
		len++;
	}

	return len;
}


static UCHAR s[22];
static CHAR * itoa(ULONG n, UCHAR base)
{
	ULONG i, j;
	UCHAR tmp[22], digit;

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
	j = strlen((CHAR *)tmp) - 1;
	for (i = 0; i < strlen((CHAR *)tmp); i++) 
	{
		s[i] = tmp[j];
		j--;
	}
	s[i] = '\0';

	return (CHAR *)s;
}


VOID VIDOUT::Write(ULONG Num, UINT Base)
{
	Write(itoa(Num, Base));
}
