/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	i386driver.cpp

Abstract:

	This is our driver interface and support functions.

Author:

	Stephen von Takach 08-Oct-2006

Revision History:

--*/


#include <def.h>
#include <arch/asmf.h>
#include <arch/i386driver.h>
#include <arch/idt.h>	// We need the defines held in here for pic control


using namespace I386;


//
// Initialise our base class
//
I386DRIVER::I386DRIVER(CHAR *Name, UINT Type, UINT Irq)
: DRIVER(Name, Type, Irq)
{}


//
// Enable the IRQ associated with this driver
//
VOID I386DRIVER::EnableIrq()
{
	BYTE Val;

	//
	// Select the controller based on the IRQ number
	//
	if(Irq <= 7)
	{
		//
		// Get the bitmap of enabled devices and mark
		//	the required bit to true and set the bitmap
		//
		Val = inportb(PIC1_PB);
		Val = Val & ~(1 << Irq);
		outportb(PIC1_PB, Val);
	}
	else
	{
		Irq = Irq - 8;
		Val = inportb(PIC2_PB);
		Val = Val & ~(1 << Irq);
		outportb(PIC2_PB, Val);
	}
}


VOID I386DRIVER::DisableIrq()
{
	BYTE Val;
	if(Irq <= 7)
	{
		//
		// Get the bitmap of enabled devices and mark
		//	the required bit to false and set the bitmap
		//
		Val = inportb(PIC1_PB);
		Val = Val | (1 << Irq);
		outportb(PIC1_PB, Val);
	}
	else
	{
		Irq = Irq - 8;
		Val = inportb(PIC2_PB);
		Val = Val | (1 << Irq);
		outportb(PIC2_PB, Val);
	}
}


VOID I386DRIVER::ClearInterrupt()
{

	//
	// Sends the EOI to both interrupt controllers depending
	//	on the irq number.
	//
	if(Irq > 7)
	{
		outportb(PIC2_PA, EOI);
	}
	outportb(PIC1_PA, EOI);
}

