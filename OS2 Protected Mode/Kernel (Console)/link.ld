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
INPUT(KStub.o Pkernel.o Support.o Video.o string.o Interrupt.o speaker.o keyboard.o kbInt.o cmos.o devices.o time.o PConsole.o)
OUTPUT(bin\Kernel.bin)
