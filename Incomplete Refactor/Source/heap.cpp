/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	heap.cpp

Abstract:

	The architecture independant part of the memory management
	system.

Author:

	Stephen von Takach 08-Oct-2006

Revision History:

--*/


#include <def.h>
#include <heap.h>


VOID *HEAP::MoreCore(UINT &TotalSize)
{
	
}


VOID *HEAP::Kmalloc(UINT Size)
{

}


VOID HEAP::Kfree(UINT Address)
{

}


VOID HEAP::InsertUsed(UINT Prev, UINT Address)
{

}


VOID HEAP::InsertFree(UINT Prev, UINT Address)
{

}


VOID *HEAP::Krealloc(UINT Block, UINT Size)
{

}


VOID *HEAP::KmallocA4(UINT Size)
{

}
