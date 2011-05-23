/*++

This code is open domain.

Module Name:

	stdlib.cpp

Abstract:

	This module contains the data to support c++ style
	 output to the screen.

Author:

	Stephen von Takach 27-April-2004

Revision History:

--*/


#include "include\stdlib.h"


namespace std
{
	OStream cout;			// Our global object.

	OStream::OStream()
	{
		write("OS Loading...\n");
	}

	OStream& OStream::operator<<(char *cp)
	{
		write(cp);

		//
		// We return *this so we can have more than one
		//	shift << on the same line eg:
		//	cout << "Hello" << endl << "hmmm\n";
		//
		return *this; 
	}

	OStream& OStream::operator<<(char cp)
	{
		put(cp);
		return *this;
	}
}
