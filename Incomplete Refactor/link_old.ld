/* Link.ld */
OUTPUT_FORMAT("binary")
ENTRY(start)
SECTIONS
{
.text 0x100000 :
{
code = .; _code = .; __code = .;
*(.text)
. = ALIGN(4096);
}

.data :
{
__CTOR_LIST__ = .; LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2) *(.ctors) LONG(0) __CTOR_END__ = .; 
__DTOR_LIST__ = .; LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2) *(.dtors) LONG(0) __DTOR_END__ = .; 

data = .; _data = .; __data = .;
*(.data)
. = ALIGN(4096);
}

.bss :
{
bss = .; _bss = .; __bss = .;
*(.bss)
. = ALIGN(4096);
}

end = .; _end = .; __end = .;
} 
OUTPUT(Bin\kernel.bin)


/*
		dc_start = .; _dc_start = .; __dc_start = .;
		*(.dtext*)				/* discardable kernel code */
		. = ALIGN(4096);
		dc_end = .; _dc_end = .; __dc_end = .;*/

		/*dd_start = .; _dd_start = .; __dd_start = .;
		*(.ddata*)				/* discardable kernel data */
		. = ALIGN(4096);
		dd_end = .; _dd_end = .; __dd_end = .;*/
