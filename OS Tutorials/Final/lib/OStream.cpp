
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\include\OStream.h"
#include "..\include\string.h"		//itoa

namespace std
{
	OStream cout;		//OStream cout

	OStream::OStream()
	{
		ibase = 10;
	}

	OStream& OStream::operator<<(char *cp)
	{
		write(cp) ;
		return *this;
	}

	OStream& OStream::operator<<(char c)
	{
		put(c) ;
		return *this;
	}

	OStream& OStream::operator<<(unsigned char *cq)
	{
		write((char *)cq) ;
		return *this;
	}

	OStream& OStream::operator<<(unsigned char c1)
	{
		put((char)c1) ;
		return *this;
	}

	OStream& OStream::operator<<(unsigned long i)
	{
		itoa((unsigned int)i, iout, ibase);
		write((char *)iout);
		return *this;
	}

	OStream& OStream::operator<<(unsigned int ii)
	{
		itoa(ii, iout, ibase);
		write((char *)iout);
		return *this;
	}

	OStream& OStream::operator<<(unsigned short int iii)
	{
		itoa((unsigned int)iii, iout, ibase);
		write((char *)iout);
		return *this;
	}

	OStream& OStream::operator<<(long in)
	{
		if(in < 0)
		{
			in *= -1;
			put('-');
		}

		itoa((unsigned int)in, iout, ibase);
		write((char *)iout);
		return *this;
	}

	OStream& OStream::operator<<(int iin)
	{
		if(iin < 0)
		{
			iin *= -1;
			put('-');
		}

		itoa((unsigned int)iin, iout, ibase);
		write((char *)iout);
		return *this;
	}

	OStream& OStream::operator<<(short int iiin)
	{
		if(iiin < 0)
		{
			iiin *= -1;
			put('-');
		}

		itoa((unsigned int)iiin, iout, ibase);
		write((char *)iout);
		return *this;
	}

	OStream& OStream::operator<<(void *pointer)
	{
		itoa((unsigned int)pointer, iout, 16);
		write((char *)iout);
		return *this;
	}

	OStream& OStream::operator<<(void (*pointerFunc)())
	{
		itoa((unsigned int)pointerFunc, iout, 16);
		write((char *)iout);
		return *this;
	}

}

void bin(std::OStream &object)
{
	object.setBase(2);
}

void oct(std::OStream &object)
{
	object.setBase(8);
}

void dec(std::OStream &object)
{
	object.setBase(10);
}

void hex(std::OStream &object)
{
	object.setBase(16);
}
