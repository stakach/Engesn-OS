/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	driver.cpp

Abstract:

	The executives driver control and base class.

Author:

	Stephen von Takach 27-July-2006

Revision History:

--*/


#include <def.h>
#include <driver.h>


DRIVER *DRIVER::Head;
DRIVER *DRIVER::Tail;


DRIVER::DRIVER(CHAR *Name, UINT Type, UINT Irq)
: Name(Name), Type(Type), Irq(Irq)
{
	if(Head == NULL)
	{
		Head = this;
		Tail = this;
		Next = NULL;
		Previous = NULL;
	}
	else
	{
		Next = NULL;
		Previous = Tail;
		Tail->Next = this;
		Tail = this;
	}
}


DRIVER::~DRIVER()
{
	if(Head == this)
	{
		Head = NULL;
		Tail = NULL;
	}
	else if(Tail == this)
	{
		Previous->Next = NULL;
		Tail = Previous;
	}
	else
	{
		Previous->Next = Next;
	}
}


DRIVER *DRIVER::GetDriver(UINT Irq)
{
	return NULL;
}

