/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	gdt.cpp

Abstract:

	Contains routines to initialise and manipulate the GDT

Author:

	Stephen von Takach 03-July-2004

Revision History:

	* Stephen von Takach 27-Feb-2005
		- GDT Pointer is now stored in the 'null' entry of the GDT to save space.

--*/


#include <def.h>
#include <arch/i386.h>
#include <arch/gdt.h>


#define GDT_EMPTY 0		//empty


using namespace I386;


DT_ENTRY GDT::Gdt[MAX_GDT];
GDT GDT::gdt;


UINT GDT::FindEmpty()
{

	//
	// Finds the first free Gdt entry and 
	//	returns 0 if there are no free descriptors.
	//

	for (UINT i = 1; i < MAX_GDT; i++)
	{
		if (Gdt[i].Dummy == GDT_EMPTY)
		{
			return i;
		}
	}
	return 0;
}


X86_SELECTOR GDT::AddDesc(ULONG Base, ULONG Limit, UINT Settings)
{

	X86_SELECTOR Selector;


	//
	// Finds first free descriptor and validates it.
	//	Returns 0 if there are no free descriptors.
	//

	UINT i = FindEmpty();

	Gdt[i].Desc.BaseLow = Base & 0xFFFF;
	Gdt[i].Desc.BaseMed = (Base >> 16) & 0xFF;
	Gdt[i].Desc.BaseHigh = Base >> 24;
	Gdt[i].Desc.LimitLow = Limit & 0xFFFF;
	Gdt[i].Desc.LimitHigh = (Limit >> 16) & 0x0F;

	Gdt[i].Desc.Access = (Settings | PRESENT) >> 8;
	Gdt[i].Desc.Granularity = ((Settings & 0xff) >> 4);


	//
	// Break down of the selector value: 0000000000011  0  00
	//									|_____________||_||__|
	//											|		|	|
	//										Index = 3	|	|___________
	//													|				|
	//	Table Indicator (TI) indicates the descriptor is in the GDT.	|
	//		A TI of 1 indicates the descriptor is in the LDT.			|
	//																	|
	//	Requester’s privilege level (RPL). 00 for kernel mode and 11 (3) for user mode.
	//	The RPL is made null here as it is up to the process creating the GDT entry
	//	what privilege level the accessing code should have.
	//

	Selector.Index = i;
	Selector.Ti = 0;
	Selector.Rpl = 0;

	return Selector;
}


VOID GDT::RemDesc(const X86_SELECTOR &Selector)
{

	//
	// Removes an entry in the Gdt.
	//

	Gdt[Selector.Index].Dummy = GDT_EMPTY;
}


GDT::GDT()
{
	//
	// Fills the pointer structure for the Gdt, validates it and loads it.
	//	The GDT Pointer is stored in the null gdt entry as this is never
	//	referenced by the cpu.
	//
	//	Note: We fill and load the GDT here as we move the stack before the
	//		main function is called. Setting up the GDT after the main function
	//		corrupts the stack and crashes the kernel.
	//

	((DT_POINTER *)Gdt)->Limit = (MAX_GDT * sizeof(DT_ENTRY) - 1);
	((DT_POINTER *)Gdt)->Base = Gdt;


	//
	// Marks all entries as free
	//

	for (UINT i = 1; i < MAX_GDT; i++)
	{
		Gdt[i].Dummy = GDT_EMPTY;
	}


	//
	// Adds the code and data descriptors with a 4gig (max) limit.
	//

	AddDesc(0, 0xFFFFFFFF, (D_CODE | D_READ | D_BIG | D_BIG_LIM));		//Kernel code descriptor
	AddDesc(0, 0xFFFFFFFF, (D_DATA | D_WRITE | D_BIG | D_BIG_LIM));		//Kernel data\stack descriptor


	//
	// Loads our GDT
	//

	asm volatile ("lgdt %0" : "=m" (Gdt));
}

