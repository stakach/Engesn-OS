/*++

This code is open domain.

Module Name:

	support.c

Abstract:

	This module implements system startup and system shutdown code.

Author:

	David Stout (whyme_t) 24-March-2003

Revision History:


--*/


void _main()
{

	//
	// Walk and call the constructors in the ctor_list
	//

	extern void (*_CTOR_LIST__)();				// the ctor list is defined in the linker script
	void (**Constructor)() = &_CTOR_LIST__;		// hold current constructor in list
	int Total = *(int *)Constructor;			// the first INT is the number of constructors

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


void _atexit()
{

	//
	// Walk and call the deconstructors in the dtor_list
	//

	extern void (*_DTOR_LIST__)();				// the dtor list is defined in the linker script
	void (**Deconstructor)() = &_DTOR_LIST__;	// hold current deconstructor in list
	int Total = *(int *)Deconstructor;			// the first INT is the number of deconstructors

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
