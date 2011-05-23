/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	memorycontrol.cpp

Abstract:

	The base class for memory management, it links a heap with
		a pager and keeps track of all the memory being used
		throughout the system.
	This file also implements the new and delete operators.

Author:

	Stephen von Takach 09-Oct-2006

Revision History:

--*/


#include <def.h>
#include <hal.h>
#include <memorycontrol.h>


MEMORYCONTROL *MEMORYCONTROL::Head = NULL;
MEMORYCONTROL *MEMORYCONTROL::Tail = NULL;
UINT MEMORYCONTROL::PageSize = 0;


MEMORYCONTROL::MEMORYCONTROL()
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


MEMORYCONTROL::~MEMORYCONTROL()
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



void* operator new(ULONG size)
{
	return HAL::KernelMm->Kmalloc((UINT)size);
}

void* operator new[](ULONG size)
{
	return HAL::KernelMm->Kmalloc((UINT)size);
}

void operator delete(void *p)
{
	HAL::KernelMm->Kfree((UINT)p);
}

void operator delete[](void *p)
{
	HAL::KernelMm->Kfree((UINT)p);
}


