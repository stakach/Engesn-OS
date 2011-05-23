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
INPUT(KStub.o Support.o OStream.o GDT.o Interrupt.o PIT.o ACPI.o bios32.o CPUDetect.o IStream.o kernel.o string.o kbInt.o keyboard.o Video.o Modes.o speaker.o cmos.o end.o asmf.o semaphore.o)
OUTPUT(bin\Kernel.bin)
