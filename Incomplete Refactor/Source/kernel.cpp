/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	kernel.cpp

Abstract:

	This module contains routines to initialise the kernel data structures.

Author:

	Stephen von Takach 05-July-2006

Revision History:

--*/


#include <hal.h>
#include <arch/i386.h>


void idle();


int main()
{
	//
	// Setup the Hardware Abstraction Layer for the kernel.
	//
	if(HAL::hal->Setup() == STATUS_SUCCESS)
	{
		HAL::Console->Write("System Loaded!");
		HAL::hal->Shutdown(SHUTDOWN);
	}

	//
	// Kernel initialisation complete
	//	Start the idle loop.
	//
	idle();

	return 0;
}


//
// This now becomes our idle process
//
void idle()
{
	while(true);
}
