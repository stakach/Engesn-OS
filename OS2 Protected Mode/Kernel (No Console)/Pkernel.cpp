#include "Pkernel.h"


Video video;
Devices devices;
System system;
Interrupt intcontrol;
Keyboard keyboard;


int main(void)
{
	system.main();
	system.restart(0); //Halts the cpu
}


System::System()
{
	time(sysTime);
	date(sysDate);

	WinMain = video.new_window(0,0, (video.scrwidth - 1), (video.scrheight - 2));
	video.enablecursor(WinMain, true);
	video.setcolour(WinMain, TCYAN | BBLUE);
	video.cls(WinMain);

	StatusBar = video.new_window(1, (video.scrheight - 1), (video.scrwidth - 10), (video.scrheight - 1));
	video.setcolour(StatusBar, TLIGHTCYAN | BGREEN);
	video.cls(StatusBar);

	video.new_window((video.scrwidth - 9), (video.scrheight - 1), (video.scrwidth - 1), (video.scrheight - 1));
	video.setcolour(StatusBar + 1, TBLACK | BGREEN);
	video.cls(StatusBar + 1);

	video.new_window(0, (video.scrheight - 1), 0, (video.scrheight - 1));
	video.setcolour(StatusBar + 2, TCYAN | BBLUE);
	video.cls(StatusBar + 2);


	video.gotoxy(WinMain, 0, 13);
	video.setcolour(WinMain, TYELLOW | BBLUE);
	video.centerText(WinMain, "THE");
	video.centerText(WinMain, "  PPPPPPP       LL       ");
	video.centerText(WinMain, "  PP     P      LL       ");
	video.centerText(WinMain, "  PP      P     LL       ");
	video.centerText(WinMain, "  PP      P     LL       ");
	video.centerText(WinMain, "  PP     P      LL       ");
	video.centerText(WinMain, "  PPPPPPP       LL       ");
	video.centerText(WinMain, "  PP            LL       ");
	video.centerText(WinMain, "  PP            LL       ");
	video.centerText(WinMain, "  PP            LL       ");
	video.centerText(WinMain, "  PP            LL       ");
	video.centerText(WinMain, "  PP            LLLLLLLLL");
	video.centerText(WinMain, "KERNEL\n");

	video.setcolour(WinMain, TCYAN | BBLUE);
	video.centerText(WinMain, "(c) 2003 - Engesn Software Corp.");
}

void System::main()
{
	int i = 0;
	enable();
	devices.printDevices(video, WinMain);
	video.putc(WinMain, '\n');
	video.putc(WinMain, keyboard.getch());
	i = 2 / i;
}

void System::restart(int Type) /*the different types of restarts*/
{
	_atexit();

	disable();					/* disable interrupts */

	keyboard.FlushBuffer();		/* clear keyboard buffer */
	
	switch (Type)				/* 0 shutdown, 1 Cold boot, Other Warm boot */
	{
	case 1:
		keyboard.SendCommand(0xFE);
	}

	halt();						/* Halt the cpu */
}

void System::assert(int Window, char expr, char *error_msg) /*Different levels of kernel errors, only one so far.*/
{
	video.setcolour(Window, TRED | BBLUE | BLINK); /*print error in red*/
	video.prints(Window, "\n  Error: ");
	video.setcolour(Window, TRED | BBLUE);
	video.prints(Window, error_msg);
	video.putc(Window, '\n');

	switch (expr)
	{
	case 1:			/*level one. No need to restart*/
			video.setcolour(Window, TCYAN | BBLUE);
			return;
	default:		/*level zero. restart required.*/
			video.prints(Window, "  System halted.");
			video.hidecursor(true);
			break;
	}
	restart(0);
}

