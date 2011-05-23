#ifndef __STDLIB_H__
#define __STDLIB_H__


#include "video.h"


namespace std
{
	const char endl = '\n';		// Define of endl

	class OStream : public Video
	{
		public:
			OStream();
			OStream& operator<<(char *cp);	// Overload the shift operator for each
			OStream& operator<<(char cp);	//	data type.
	};

	extern OStream cout;		// Our global cout object.
}


#endif
