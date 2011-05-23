#ifndef __OSTREAM_H__
#define __OSTREAM_H__

#include "Video.h"

namespace std
{
//---------------OStream:------------------------

	const char endl = '\n';
	const char ends = '\0';

	class OStream : public Video
	{
		public:
			OStream();

			OStream& operator << (char *cp);
			OStream& operator << (char c);
			OStream& operator << (unsigned char *cq);
			OStream& operator << (unsigned char c1);
			OStream& operator << (unsigned long i);
			OStream& operator << (unsigned int ii);
			OStream& operator << (unsigned short int iii);
			OStream& operator << (long in);
			OStream& operator << (int iin);
			OStream& operator << (short int iiin);

			OStream& operator << (void *pointer);
			OStream& operator << (void (*pointerFunc)());
			OStream& operator << (void (*MemberFunc)(OStream &))
			{
				MemberFunc(*this);
				return *this;
			}

			void setBase(unsigned char Base)
			{
				ibase = Base;
			};

		private:
			unsigned char iout[22], ibase;

	};

	extern OStream cout;
}

void bin(std::OStream &object);
void oct(std::OStream &object);
void dec(std::OStream &object);
void hex(std::OStream &object);

#endif

