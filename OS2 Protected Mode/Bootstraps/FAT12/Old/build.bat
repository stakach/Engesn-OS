@echo off
echo This script builds the Engesn/os1 Bootstrap from source. You will
echo need NASM all installed and within reach of your PATH variable.
echo Building 1st stage bootloader...
echo -
c:\nasm\nasmw -f obj -l bootf.lst bootf.asm
c:\nasm\jloc\jloc bootf.lnk bootf.bin
c:\nasm\jloc\partcopy bootf.bin 0 3 -f0 0
c:\nasm\jloc\partcopy bootf.bin 3E 1C2 -f0 3E
del bootf.bin
del bootf.obj
echo Done.
pause
@echo on
