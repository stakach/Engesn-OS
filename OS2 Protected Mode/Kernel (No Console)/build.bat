@echo off
echo Building Kernel...
echo -

C:\DJGPP\bin\gcc -c Video.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions 
C:\DJGPP\bin\gcc -c Pkernel.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions
C:\DJGPP\bin\gcc -c Support.c -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions
C:\DJGPP\bin\gcc -c string.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions
C:\DJGPP\bin\gcc -c speaker.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions
C:\DJGPP\bin\gcc -c keyboard.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions
C:\DJGPP\bin\gcc -c kbInt.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions
C:\DJGPP\bin\gcc -c cmos.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions
C:\DJGPP\bin\gcc -c devices.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions
C:\DJGPP\bin\gcc -c time.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions
C:\DJGPP\bin\gcc -c Interrupt.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions

c:\nasm\nasmw -f aout Pkernel_Start.asm -o KStub.o

C:\DJGPP\bin\ld -T link.ld

del Pkernel.o
del Video.o
del Support.o
del KStub.o
del speaker.o
del string.o
del keyboard.o
del kbInt.o
del cmos.o
del devices.o
del time.o
del Interrupt.o

echo Done.
pause
@echo on
