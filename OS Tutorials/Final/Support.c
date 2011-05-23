//Support.c


void _main()	//Walk and call the constructors in the ctor_list
{
	extern void (*_CTOR_LIST__)() ;				//the ctor list is defined in the linker script
	void (**constructor)() = &_CTOR_LIST__ ;	//hold current constructor in list
	int total = *(int *)constructor ;			//the first int is the number of constructors
	
	constructor++ ;								//increment to first constructor
	
	while(total)
	{
		(*constructor)() ;
		total-- ;
		constructor++ ;
	}
}

void _atexit()	//Walk and call the deconstructors in the dtor_list
{
	extern void (*_DTOR_LIST__)() ;				//the dtor list is defined in the linker script
	void (**deconstructor)() = &_DTOR_LIST__ ;	//hold current deconstructor in list
	int total = *(int *)deconstructor ;			//the first int is the number of deconstructors

	deconstructor++ ;							//increment to first deconstructor

	while(total)
	{
		(*deconstructor)() ;
		total-- ;
		deconstructor++ ;
	}
}

