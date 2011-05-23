/*++

This code is open domain.

Module Name:

	video.cpp

Abstract:

	This module contains routines to print data to the screen.

Author:

	David Stout (whyme_t) 24-March-2003

Revision History:

	* Stephen von Takach 27-April-2004
		- Comments and minor improvements (newline char support),
			(Improved constructor), (Improved write function).

--*/


#include "include\video.h"


Video::Video()
{

	//
	// Sets up the pointer to video memory and
	//	clears the screen to set the position
	//	variables.
	//
	videomem = (unsigned short*)0xb8000;
	clear();
}


void Video::clear()
{

	//
	// Sets every character on the screen to be
	//	a black (0x700) space (' ') and then
	//	re-sets the position variables.
	//
	unsigned int i;

	for(i = 0; i < (80*25); i++)
	{
		videomem[i] = (unsigned char) ' ' | 0x0700;
	}

	x = 0;
	y = 0;
}


void Video::write(char *cp)
{

	//
	// Calls put for every character in the string
	//	*cp.
	//
	for (; *cp; cp++)
	{
		put(*cp);
	}
}

void Video::put(char c)
{

	//
	// Places a character on the screen in the
	//	next position.
	//
	if(x >= 80)
	{
		//
		// Checks if we are at the end of the line.
		//	and adjusts the variables accordingly.
		//
		x = 0;
		y += 1;
	}

	if(y >= 25)
	{
		//
		// If we are at the end of the screen just
		//	clear it to continue printing.
		//
		clear();
	}

	if(c == '\n')
	{
		//
		// Checks for the new line character and
		//	otherwise just prints the character.
		//
		x = 0;
		y += 1;
	}
	else
	{
		//
		// The screen is a linear array of 80x25
		//	characters with colour codes.
		//
		videomem[y * 80 + x] = (unsigned char) c | 0x0700;
		x++;
	}
}
