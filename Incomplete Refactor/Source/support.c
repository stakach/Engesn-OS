/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	support.c

Abstract:

	This module implements system startup and system shutdown code.

Author:

	David Stout (whyme_t) 24-March-2003

Revision History:

--*/


#include <def.h>
#include <arch/asmf.h>


VOID _main()
{

	//
	// Walk and call the constructors in the ctor_list
	//

	extern VOID (*_CTOR_LIST__)();				// the ctor list is defined in the linker script
	VOID (**Constructor)() = &_CTOR_LIST__;		// hold current constructor in list
	INT Total = *(INT *)Constructor;			// the first INT is the number of constructors

	//
	// increment constructors and call them
	//

	Constructor++;
	
	while(Total)
	{
		(*Constructor)();
		Total--;
		Constructor++;
	}
}

VOID _atexit()
{

	//
	// Walk and call the deconstructors in the dtor_list
	//

	extern VOID (*_DTOR_LIST__)();				// the dtor list is defined in the linker script
	VOID (**Deconstructor)() = &_DTOR_LIST__;	// hold current deconstructor in list
	INT Total = *(INT *)Deconstructor;			// the first INT is the number of deconstructors

	//
	// increment deconstructors and call them.
	//

	Deconstructor++;

	while(Total)
	{
		(*Deconstructor)();
		Total--;
		Deconstructor++;
	}
}


