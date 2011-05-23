CC = gcc
CFLAGS = -nostdlib -fno-builtin -fno-exceptions -ffast-math
OBJECTS = loader.o support.o video.o stdlib.o kernel.o

Bin\kernel.bin : $(OBJECTS)
	C:\DJGPP\bin\ld -T link.ld

%.o : Source\%.c
	$(CC) $(CFLAGS) -ffreestanding -c $<

%.o : Source\%.cpp
	$(CC) $(CFLAGS) -fno-rtti -c $<

loader.o : Source\loader.asm
	c:\nasm\nasmw -f aout Source\loader.asm -o loader.o
