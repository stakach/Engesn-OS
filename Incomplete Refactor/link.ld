/* Link.ld */
OUTPUT_FORMAT("binary")
ENTRY(start)
SECTIONS 
{
	.text 0x100000 :
	{
		code = .; _code = .; __code = .;	/* symbols to mark start of section */
		*(.text*)				/* wildcard for "gcc -ffunction-sections" */
		*(.gnu.linkonce.t.*)			/* C++ templates? */
		*(.rodata*)				/* read-only data (ELF only) */
		*(.gnu.linkonce.r.*)
		. = ALIGN(4096);
	}

	.data :
	{
			/* C++ static constructors and destructors */
		__CTOR_LIST__ = .; LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2) *(.ctors) LONG(0) __CTOR_END__ = .; 
		__DTOR_LIST__ = .; LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2) *(.dtors) LONG(0) __DTOR_END__ = .; 

		data = .; _data = .; __data = .;

		*(.data*)				/* wildcard for "gcc -fdata-sections" */
		*(.gnu.linkonce.d.*)
		. = ALIGN(4096);
	}

	.bss :
	{
		bss = .; _bss = .; __bss = .;

		*(.bss*)				/* wildcard for "gcc -fdata-sections" */
		*(.gnu.linkonce.b.*)
		*(COMMON)				/* "common" variables */
		. = ALIGN(4096);
	}

		/* MinGW debug sections
			Omit these and you may get an invalid executable file */
	.stab :
	{
		*(.stab)
	}
	.stabstr :
	{
		*(.stabstr)
	}

	end = .; _end = .; __end = .;
}
OUTPUT(Bin\kernel.bin)
