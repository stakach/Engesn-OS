@echo off
echo Building Kernel...
echo -

rem  -Weffc++ //Warns about c++ efficiency.

rem  ------>> Keyboard.cpp must have no optimisations or will not function correctly!
rem  I would not recommend optimisations on any files that are being updated regularly.

rem  ** Note: Disable optismisations and fast maths for debugging or if you have any troubles.

C:\DJGPP\bin\gcc -c kernel.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c Support.c -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c end.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c semaphore.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c asmf.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c drv\vid\Video.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c drv\vid\Modes.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c drv\keyb\keyboard.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -fomit-frame-pointer
C:\DJGPP\bin\gcc -c drv\keyb\kbInt.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3
C:\DJGPP\bin\gcc -c lib\string.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c lib\OStream.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c lib\IStream.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c x86\Interrupt.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3
C:\DJGPP\bin\gcc -c x86\GDT.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c x86\PIT.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3
C:\DJGPP\bin\gcc -c x86\speaker.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c x86\cmos.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c x86\bios32.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c x86\ACPI.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer
C:\DJGPP\bin\gcc -c x86\CPUDetect.cpp -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffast-math -O3 -fomit-frame-pointer

c:\nasm\nasmw -f aout kernel_Start.asm -o KStub.o

C:\DJGPP\bin\ld -T link.ld

del kernel.o
del Video.o
del Support.o
del KStub.o
del string.o
del keyboard.o
del kbInt.o
del Interrupt.o
del OStream.o
del IStream.o
del asmf.o
del GDT.o
del PIT.o
del speaker.o
del cmos.o
del end.o
del bios32.o
del ACPI.o
del semaphore.o
del Modes.o
del CPUDetect.o

echo Done.
pause
@echo on
