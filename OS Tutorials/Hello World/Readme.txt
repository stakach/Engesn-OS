Tutorial Group 1: "Hello World" Kernel.
---------------------------------------

1) The source files can be found in the 'Source' directory. 

2) A compiled binary is avaliable in the 'Bin' directory.


*** (Assumes gcc compiler or djgpp distribution being used)
 *
3) To compile the kernel you must first:
	a. Open the 'makefile' file in notepad (or similar) and
	   modify the paths to 'LD' (The linker) and 'nasm'
	   (An assembly compiler).

	b. Run 'Build.bat' (windows) or use make to run the
	   makefile (Windows / linux).


4) The kernel is multiboot compliant so can be booted off a floppy
    with Grub installed or from any bootloader that sets up
    protected mode and loads the kernel to the 1Mb mark.

    'menu.lst' is the menu for a Grub floppy with the fat file
    file system. It will boot the kernel if placed it is placed
    in the 'boot' directory of the Grub floppy.

	You can find a grub floppy image with this tutorial:
		grubfloppy.ima
	*Note: his floppy already contains the kernel file and
		the menu.lst file, its ready to boot.
