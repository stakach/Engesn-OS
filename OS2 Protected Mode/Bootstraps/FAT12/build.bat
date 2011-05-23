@echo off
echo This script builds the Engesn/os1 Bootstrap from source. You will
echo need NASM all installed and within reach of your PATH variable.
echo Building 1st stage bootloader...
echo -
c:\nasm\nasmw -f obj -l bootf.lst bootf.asm
c:\nasm\jloc\jloc bootf.lnk bootstrap
FloppyWriter.exe
del bootstrap
del bootf.obj
echo Done.
pause
@echo on
