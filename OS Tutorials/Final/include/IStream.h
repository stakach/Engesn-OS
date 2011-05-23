#ifndef __ISTREAM_H__
#define __ISTREAM_H__

#include "keyboard.h"
#include "speaker.h"

namespace std
{
//---------------IStream:------------------------

	const int BUFFSIZE = 256;

	class IStream : public Keyboard, private Speaker
	{
		public:
			IStream()
			{
				ibase = 10;
			};
			IStream& operator >> (char *cp);
			IStream& operator >> (char &c);
			IStream& operator >> (unsigned char *ucp);
			IStream& operator >> (unsigned char &uc);
			IStream& operator >> (unsigned long &l);
			IStream& operator >> (unsigned int &i);
			IStream& operator >> (unsigned short int &ii);
			IStream& operator >> (long &sl);
			IStream& operator >> (int &si);
			IStream& operator >> (short int &sii);

			IStream& operator >> (void (*MemberFunc)(IStream &))
			{
				MemberFunc(*this);
				return *this;
			}

			void setBase(unsigned char Base)
			{
				ibase = Base;
			};

		private:
			char buffer[BUFFSIZE];

			unsigned int getString();
			void getWord(char *str1, char *p1);

			char *chptr;
			char ibase;
	};

	extern IStream cin;
}

void bin(std::IStream &object);
void oct(std::IStream &object);
void dec(std::IStream &object);
void hex(std::IStream &object);

#endif

