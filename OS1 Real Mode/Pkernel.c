/*
	Pkernel.c
	---------
	(c) Engesn Software Corp.
    Kernel and console for Engesn\OS1 Ver 0.07

*/

#include "Pkernel.h"

unsigned int A20E; /*memory sizes and A20 gate value*/
time_st_t sysTime; /*System start time*/
date_st_t sysDate; /*System start date*/

int StatusBar;

void k_main() /*The kernel!! Well, it dosn't do much.*/
{
	int WinMain;	/*The token for the main text window*/

	if(A20_BIT != GET_A20_STATE())
	{
		A20_ENABLE();
	}


	vidSetup(); /*determines if vga card and sets variables for direct vid access*/


	get_time(&sysTime);
	get_date(&sysDate);


	WinMain = new_window(0,0, (scrwidth - 1), (scrheight - 2));
	enablecursor(WinMain, TRUE);
	tty_setcolour(WinMain, TCYAN | BBLUE);
	cls(WinMain);

	StatusBar = new_window(1, (scrheight - 1), (scrwidth - 10), (scrheight - 1));
	tty_setcolour(StatusBar, TLIGHTCYAN | BGREEN);
	cls(StatusBar);

	new_window((scrwidth - 9), (scrheight - 1), (scrwidth - 1), (scrheight - 1));
	tty_setcolour(StatusBar + 1, TBLACK | BGREEN);
	cls(StatusBar + 1);

	new_window(0, (scrheight - 1), 0, (scrheight - 1));
	tty_setcolour(StatusBar + 2, TCYAN | BBLUE);
	cls(StatusBar + 2);


	gotoxy(WinMain, 0, 13);
	tty_setcolour(WinMain, TYELLOW | BBLUE);
	centerText(WinMain, "THE");
	centerText(WinMain, "  PPPPPPP       KK      K");
	centerText(WinMain, "  PP     P      KK     KK");
	centerText(WinMain, "  PP      P     KK   KK  ");
	centerText(WinMain, "  PP      P     KK KK    ");
	centerText(WinMain, "  PP     P      KKK      ");
	centerText(WinMain, "  PPPPPPP       KKKK     ");
	centerText(WinMain, "  PP            KK  K    ");
	centerText(WinMain, "  PP            KK  Kk   ");
	centerText(WinMain, "  PP            KK   KK  ");
	centerText(WinMain, "  PP            KK    K  ");
	centerText(WinMain, "  PP            KK     KK");
	centerText(WinMain, "KERNEL\n");

	tty_setcolour(WinMain, TCYAN | BBLUE);
	centerText(WinMain, "(c) 2003 - Engesn Software Corp.");


	deviceSetup(); /*Detects other devices such as floppy drives and memory*/
	kbSetup();  /*resets keyboard and sets scan rates and delays*/


	if(extmem_size == 0 || A20E == 0)  /*Check if extended mem exists and A20 gate enabled*/ 
	{
		assert(WinMain, 1,"Unable to access or not enough avaliable memory");
	}
	
	printDevices(WinMain);

	restart(startConsole(WinMain)); /*once it returns from 'console' it will shutdown*/
}

void restart(int Type) /*the different types of restarts*/
{
	unsigned long Boot = 0xFFFFFFF0;
	unsigned int far *BiosMem = (unsigned int far *)0x00400072L;

	disable();					/* disable interrupts */
	
	switch (Type)			/* 0 shutdown, 1 Cold boot, Other Warm boot */
	{
	case 1:
			*BiosMem = 0x0000;	/* Cold boot */
			break;
	default:
			*BiosMem = 0x1234;	/* Warm boot */
			break;
	}


	if(!(Type == 0))		/* pulse the CPU reset line (if not power off) */
	{
		((void (far *)())Boot)();
	}

	kbFlushBuffer();			/* clear keyboard buffer */

	halt();						/* Halt the cpu */
}

void assert(int Window, char expr, char *error_msg) /*Different levels of kernel errors, only one so far.*/
{
	tty_setcolour(Window, TRED | BBLUE | BLINK); /*print error in red*/
	prints(Window, "  Error: ");
	tty_setcolour(Window, TRED | BBLUE);
	prints(Window, error_msg);
	tty_putc(Window, '\n');

	switch (expr) 
	{
	case 1:			/*level one. No need to restart*/
			tty_setcolour(Window, TCYAN | BBLUE);
			return;
	default:		/*level zero. restart required.*/
			break;
	}
	restart(0);
}

void Delay(unsigned int MilliSecs) /*Delays execution of code for a period of time*/
{
    MicroDelay(MilliSecs * 100);
}

