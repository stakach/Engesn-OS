/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	hal.cpp

Abstract:

	Holds the static pointers to the hardware interfaces.

Author:

	Stephen von Takach 27-July-2006

Revision History:

--*/


#include <def.h>
#include <hal.h>	// Interface Class Definitions


DEBUG_CONSOLE *HAL::Console;
MEMORY *HAL::KernelMm;
HAL *HAL::hal;

