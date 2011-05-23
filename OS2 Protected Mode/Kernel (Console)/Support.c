//Support.c
void _main()
{
	//Walk and call the constructors in the ctor_list

	//the ctor list is defined in the linker script
	extern void (*_CTOR_LIST__)() ;

	//hold current constructor in list
	void (**constructor)() = &_CTOR_LIST__ ;

	//the first int is the number of constructors
	int total = *(int *)constructor ;
	
	//increment to first constructor
	constructor++ ;
	
	while(total)
	{
		(*constructor)() ;
		total-- ;
		constructor++ ;
	}
}

void _atexit()
{
	//Walk and call the deconstructors in the dtor_list

	//the dtor list is defined in the linker script
	extern void (*_DTOR_LIST__)() ;

	//hold current deconstructor in list
	void (**deconstructor)() = &_DTOR_LIST__ ;

	//the first int is the number of deconstructors
	int total = *(int *)deconstructor ;

	//increment to first deconstructor
	deconstructor++ ;

	while(total)
	{
		(*deconstructor)() ;
		total-- ;
		deconstructor++ ;
	}
}

