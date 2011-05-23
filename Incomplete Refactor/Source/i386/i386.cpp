/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	i386.cpp

Abstract:

	The i386 hardware specific kernel routines

Author:

	Stephen von Takach 27-July-2006

Revision History:

--*/


#include <def.h>
#include <hal.h>
#include <arch/i386.h>
#include <arch/gdt.h>
#include <arch/mboot.h>
#include <arch/pager.h>
#include <debug/video.h>


	#ifdef __cplusplus
		extern "C" 
		{
	#endif 

			//
			// If for some reason a v-table
			//	entry is null we call this function
			//	this should never happen.
			//
			VOID __cxa_pure_virtual();

	#ifdef __cplusplus
		}
	#endif


using namespace I386;


I386HAL I386HAL::SystemHal;


I386HAL::I386HAL()
{
	KernelMm = &PAGER::KernelMM;
	hal = this;
}


KE_STATUS I386HAL::Setup()
{
	//
	// Install the interfaces for the kernel then
	//	return the status.
	//
	Console = &DEBUG::VIDOUT::Vidout;

	//
	// Finds multi-boot compliant information.
	//	This is used to map memory.
	//
	KE_STATUS SetupDone = CheckMboot();

	return SetupDone;
}


VOID I386HAL::Shutdown(SHUTDOWN_TYPE Type)
{
	//
	// Terminate the HAL in the requested fashion.
	//
	Terminate(Type);
}


MEMORY *I386HAL::GetAddressSpace(BOOL UserMode)
{
	return NULL;
}


VOID I386HAL::CriticalException(CHAR *ErrorMsg, INT ErrorCode, INT ErrorType)
{
	//
	// Displays an error message and either returns or terminates the machine.
	//

	DEBUG::VIDOUT::Vidout.SetTextColour(RED);
	Console->Write("  A '");
	Console->Write(ErrorMsg);
	Console->Write("' has occured.\n");
	DEBUG::VIDOUT::Vidout.SetTextColour(WHITE);
	switch (ErrorType)
	{
		case 1:
			return;
		default:
			Terminate(SHUTDOWN);
	}
}


VOID __cxa_pure_virtual()
{
	//
	//	::GCC (Specific)
	// A nothing function if a pure virtual function is called.
	//	We just halt the cpu if we find our way here.
	//

	I386HAL::SystemHal.CriticalException("Pure virtual function called with no v-table entry", 35, 0);
}




