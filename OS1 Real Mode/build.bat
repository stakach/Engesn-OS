@echo off
echo Building Kernel...
echo -
C:\TC\tcc -c Pkernel.c
C:\TC\tcc -c dispIO.c
C:\TC\tcc -c PConsole.c
C:\TC\tcc -c cmos.c
C:\TC\tcc -c keyboard.c
C:\TC\tcc -c time.c
C:\TC\tcc -c video.c
C:\TC\tcc -c string.c
C:\TC\tcc -c speaker.c
C:\TC\tcc -c devices.c
C:\TC\tcc -c vect.c
c:\nasm\nasmw PKernel_stub.asm -f obj
c:\nasm\nasmw asmf.asm -f obj
c:\nasm\nasmw a20.asm -f obj
echo ALL: > jloc.conf
echo	Pkernel_stub.obj >> jloc.conf
echo 	Pkernel.obj >> jloc.conf
echo	a20.obj >> jloc.conf
echo	asmf.obj >> jloc.conf
echo	vect.obj >> jloc.conf
echo	cmos.obj >> jloc.conf
echo	keyboard.obj >> jloc.conf
echo 	string.obj >> jloc.conf
echo	time.obj >> jloc.conf
echo	video.obj >> jloc.conf
echo 	dispIO.obj >> jloc.conf
echo 	devices.obj >> jloc.conf
echo 	speaker.obj >> jloc.conf
echo 	PConsole.obj >> jloc.conf
echo Loader: 0 0 0 >> jloc.conf
echo	* >> jloc.conf
c:\nasm\jloc\jloc jloc.conf bin\PKernel.bin
del Pkernel.obj
del Pkernel_stub.obj
del dispIO.obj
del PConsole.obj
del asmf.obj
del a20.obj
del cmos.obj
del keyboard.obj
del time.obj
del string.obj
del speaker.obj
del video.obj
del devices.obj
del vect.obj
del jloc.conf
echo Done.
pause
@echo on
