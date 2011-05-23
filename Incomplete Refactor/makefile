CC = gcc
CP = gpp
CFLAGS = -nostdlib -fno-builtin -fno-exceptions -ffast-math -fomit-frame-pointer -I Source\include
OBJECTS = loader.o support.o video.o kernel.o gdt.o idt.o driver.o i386driver.o pager.o memorycontrol.o heap.o hal.o i386.o terminate.o mboot.o

bin\kernel.bin : $(OBJECTS)
	ld $(OBJECTS) -T link.ld

%.o : Source\%.c
	$(CC) $(CFLAGS) -ffreestanding -c $<

%.o : Source\%.cpp
	$(CP) $(CFLAGS) -fno-rtti -c $<

%.o : Source\debug\%.cpp
	$(CP) $(CFLAGS) -fno-rtti -c $<

%.o : Source\i386\%.cpp
	$(CP) $(CFLAGS) -fno-rtti -c $<

loader.o : Source\loader.asm
	nasmw -f aout Source\loader.asm -o loader.o
