/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	idt.cpp

Abstract:

	Contains routines to initialise and manipulate the IDT

Author:

	Stephen von Takach 01-July-2004

Revision History:

	* Stephen von Takach 03-July-2004
		- Use of a common gate/descriptor structure DT_ENTRY.

--*/


#include <def.h>
#include <hal.h>
#include <arch/i386.h>
#include <arch/asmf.h>
#include <arch/idt.h>


namespace
{


	VOID DefaultInt()
	{

		//
		// Catches any unexpected interrupts.
		//

		asm volatile ("leave");	// Leave used to exit the current frame.
		asm volatile ("iret");	// Iret returns from the interrupts.
	}


}


//
// Defines PIC interrupt numbers. See 'Design Ideas.txt'
//
#define MAS_VEC	32
#define SLA_VEC	40


using namespace I386;


// Static class member variables.
//DT_ENTRY IDT::Idt[MAX_IDT];
//DT_POINTER IDT::IdtDesc;
//INT IDT::NextFreeInterrupt;
IDT IDT::idt;


IDT::IDT()
{
	//
	// Clear the IDT structure
	//
	for(UINT i = 0; i < MAX_IDT; i++)
	{
		AddInt(i, (DWORD)DefaultInt, (PRESENT | RING_3 | INT_G));
	}


	//
	// Remap the PIC and Get first free interrupt number
	//
	RemapPic(MAS_VEC, SLA_VEC);
	NextFreeInterrupt = SLA_VEC + 9;


	//
	// Load our Idt.
	//
	IdtDesc.Limit  = MAX_IDT * (sizeof(DT_ENTRY) - 1);
	IdtDesc.Base   = Idt;
	asm volatile ("lidt %0" : "=m" (IdtDesc));
}


VOID IDT::RemapPic(UINT Pic1, UINT Pic2)
{

	//
	// Re-programs the PIC.
	// IRQ interrupts start at Pic1.
	//
	outportb(PIC1_PA, ICW1);
	outportb(PIC2_PA, ICW1);

	outportb(PIC1_PB, Pic1);
	outportb(PIC2_PB, Pic2);

	outportb(PIC1_PB, 4);
	outportb(PIC2_PB, 2);

	outportb(PIC1_PB, ICW4);
	outportb(PIC2_PB, ICW4);

	//
	// Disables interrupts, line 2 (PIC Slave) is left enabled.
	//
	outportb(PIC1_PB, 0xFB);
	outportb(PIC2_PB, 0xFF);
}


KE_STATUS IDT::AddInt(UINT Number, DWORD Handler, DWORD Settings)
{
	if(Number >= MAX_IDT)
	{
		return STATUS_FAILURE;
	}


	//
	// Set values of int
	//

	Idt[Number].Gate.LowOffset	= (Handler & 0xFFFF);
	Idt[Number].Gate.Selector	= get_cs();
	Idt[Number].Gate.Access		= Settings;
	Idt[Number].Gate.HighOffset	= (Handler >> 16);

	return STATUS_SUCCESS;
}


INT IDT::AddInt(DWORD Handler, INT AttachToIRQ)
{

	//
	// Are we pointing to an IRQ or System Call?
	//
	if(AttachToIRQ > -1)
	{
		//
		// Check IRQ is in range
		//
		if(AttachToIRQ < 16)
		{
			//
			// Calculate the IRQ's interrupt table entry
			//
			if(AttachToIRQ > 7)
				AttachToIRQ = AttachToIRQ - 8 + SLA_VEC;
			else
				AttachToIRQ = AttachToIRQ + MAS_VEC;

			//
			// Add the interrupt
			//
			if(AddInt(AttachToIRQ, Handler, (PRESENT | RING_0 | INT_G)))
				return AttachToIRQ;
		}
	}
	else if(AddInt(NextFreeInterrupt, Handler, (PRESENT | RING_0 | INT_G)))
	{
		//
		// Else we set the next free interrupt
		//
		NextFreeInterrupt++;
		return (NextFreeInterrupt - 1);
	}

	//
	// If there is an error.
	//
	return -1;
}


VOID IDT::RemInt(UINT Number)
{

	//
	// Sets interrupt to default.
	//

	AddInt(Number, (DWORD)DefaultInt, (PRESENT | RING_3 | INT_G));
}


UINT IDT::NumEntries()
{
	return MAX_IDT;
}

