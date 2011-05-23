CC = gcc
CFLAGS = -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math
OBJECTS = KStub.o Support.o OStream.o GDT.o Interrupt.o PIT.o ACPI.o bios32.o CPUDetect.o IStream.o kernel.o string.o kbInt.o keyboard.o Video.o Modes.o speaker.o cmos.o end.o asmf.o semaphore.o

bin\kernel.bin : $(OBJECTS)
	C:\DJGPP\bin\ld -T link.ld

%.o : %.c
	$(CC) $(CFLAGS) -c $<

KStub.o : kernel_Start.asm
	c:\nasm\nasmw -f aout kernel_Start.asm -o KStub.o
