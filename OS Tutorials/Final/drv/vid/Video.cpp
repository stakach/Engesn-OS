
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\..\include\Video.h"
#include "..\..\include\asmf.h"			//outportb, inportb
#include "..\..\include\semaphore.h"	//semStart, semEnd

Video::Video()
{
	colour = 0x17;		//0x7 (old Values)
	text = 0x7;
	back = 0x1;			//0x0 

	setMode(m8x50);

	clear();			//clear sets xpos and ypos
}

Video::~Video() {}

void Video::clear()		//Sets all video memory to display ' ' (blank)
{
	unsigned int i;

	for(i = 0; i < (scrWidth * scrHeight); i++)
	{
		videomem[i] = (unsigned char) ' ' | (colour << 8) ;
	}

	gotoxy(0, 0);
}

void Video::write(char *cp)		//Puts every char in a string onto the screen
{
	while((*cp) != 0) put(*(cp++));
}

void Video::put(char c)
{
	int t = xpos + ypos * scrWidth;		// get linear address
	int tab;

	switch(c)
	{
	case '\n':							// -> newline (with implicit cr)
		ypos++;

	case '\r':							// -> carriage return
		xpos = 0;
		break;

	case '\t':							// -> Calculate tab position
		tab = (t % scrWidth) % 8;
		t += 16 - tab;
		xpos += (8 - tab);
		break;

	case 8:								// -> backspace 
		if(t > 0) t--;
										// if not in home position, step back 
		if(xpos > 0)
		{
			xpos--;
		}
		else if(ypos > 0)
		{
			ypos--;
			xpos = scrWidth - 1;
		}

		*(videomem + t) = ' ' | (colour << 8);	// put space under the cursor 
		break;

	default:							// -> all other characters 
		if(c < ' ') break;				// ignore non printable ascii chars 
		*(videomem + t) = c | (colour << 8);
		xpos++;							// step cursor one character 
		if(xpos == scrWidth)			// to next line if required
		{
			xpos = 0;
			ypos++;
		}
		break;
	}

	if(ypos == scrHeight)			// the cursor moved off of the screen?
	{
		scrollup();					// scroll the screen up
		ypos--;						// and move the cursor back
	}
									// and finally, set the cursor
	setcursor();
}


void Video::scrollup()		// scroll the screen up one line
{
	int t;

	semStart();	//this memory operation should not be interrupted,
				//can cause errors (more of an annoyance than anything else)
	for(t = 0; t < scrWidth * (scrHeight - 1); t++)		// scroll every line up
		*(videomem + t) = *(videomem + t + scrWidth);
	for(; t < scrWidth * scrHeight; t++)				//clear the bottom line
		*(videomem + t) = ' ' | (colour << 8);

	semEnd();
}

void Video::setcursor()				//Hardware move cursor
{
    unsigned short offset;

	semStart();

	offset = xpos + ypos * scrWidth;	// 80 characters per line
	outportb(crtc_mem, 14);				// MSB of offset to CRTC reg 14
	outportb(crtc_mem + 1, offset >> 8);
	outportb(crtc_mem, 15);				// LSB of offset to CRTC reg 15
	outportb(crtc_mem + 1, offset);

	semEnd();
}

void Video::gotoxy(unsigned x, unsigned y)	//Software move cursor
{
	xpos = x; ypos = y;			// set cursor to location
	setcursor();				// call the hw. cursor setup
}


void Video::SetTextColour(Colours col)
{						//Sets the colour of the text being displayed
	text = (unsigned char)col;
	colour = (back << 4) | text;
}

void Video::SetBackColour(Colours col)
{						//Sets the colour of the background being displayed
	if(col > 7)
	{
		col = BLACK;
	}
	back = (unsigned char)col;
	colour = (back << 4) | text;
}

unsigned char Video::GetTextColour()
{						//Sets the colour of the text currently set
	return (unsigned char)text;
}

unsigned char Video::GetBackColour()
{						//returns the colour of the background currently set
	return (unsigned char)back;
}

void Video::SetColour(Colours Text, Colours Back, bool blink)
{						//Sets the colour of the text being displayed
	SetTextColour(Text);
	SetBackColour(Back);
	if(blink)
	{
		colour = (back << 4) | text | 128;
	}
}

void Video::cursorVisible(bool status)
{						//Displays or hides the text cursor
	semStart();

	if(status)
	{
		outportb(crtc_mem, 0x0A);
		outportb(crtc_mem + 1, (inportb(crtc_mem + 1) & 0x1F));
		setcursor();
	}
	else
	{
		outportb(crtc_mem, 0x0A);
		outportb(crtc_mem + 1, (inportb(crtc_mem + 1) | 0x20));
	}

	semEnd();
}

