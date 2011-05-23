/*
	PConsole.cpp
	------------
	(c) Engesn Software Corp.
    Text input and command interpreter for Engesn\OS2

*/

#include "PConsole.h"


extern Video video;
extern Devices devices;
extern System system;
extern Keyboard keyboard;


unsigned int Console::start(int Window) /*Engesn temporary input console*/
{
	size_t control = 0;
	size_t bs = 0;
	uchar c;
	uchar buffer[BUFFYSIZE];
	uchar par[5][BUFFYSIZE];

	buffer[control] = '\0';
	video.prints(Window, "\n> ");
	video.prints(system.StatusBar, "\n For help with commands type 'help'.");
	keyboard.FlushBuffer();

		while(1)
		{
			video.cls(system.StatusBar + 1);
			dispTimeDate(system.StatusBar + 1, (unsigned char*)"t\0", '\0');

			c = keyboard.getch();
			if(c == '\r')
			{
				ushort i;
				video.putc(Window, '\n');
				buffer[control] = '\0';
				buffer[control + 1] = '\0';

				strsplit(buffer, par[0], par[1], par[2], par[3], par[4]);

				if(!memcmp(buffer, "ver", 3)) /*Displays version number*/
				{
					video.prints(Window, "\n    EngesnOS/2 Ver: ");
					ntostr(major, str);
					video.prints(Window, str);
					video.putc(Window, '.');
					if(minor < 10)
					{
						video.putc(Window, '0');
					}
					ntostr(minor, str);
					video.prints(Window, str);
					if(!memcmp(par[1], "build", 5)) /*Displays build number*/
					{
						ntostr(build, str);
						video.prints(Window, "\n             Build: ");
						video.prints(Window, str);
					}
					video.prints(Window, "\n\n");
				}
				else if(!memcmp(buffer, "mem", 3)) /*Displays memory statistics*/
				{
					video.prints(Window, "     Low Mem: ");
					ntostr(devices.convmem_size, str);
					video.prints(Window, str);
					video.prints(Window, "KB");
					video.prints(Window, "\n    High Mem: ");
					ntostr(devices.extmem_size, str);
					video.prints(Window, str);
					video.prints(Window, "KB");
					video.putc(Window, '\n');
				}
				else if(!memcmp(buffer, "time", 4)) /*Displays system time*/
				{
					video.prints(Window, "    ");
					dispTimeDate(Window, par[0], par[1]);
					video.putc(Window, '\n');
				}
				else if(!memcmp(buffer, "date", 4)) /*Displays system Date*/
				{
					video.prints(Window, "    ");
					dispTimeDate(Window, par[0], par[1]);
					video.putc(Window, '\n');
				}
				else if(!memcmp(buffer, "kbrate", 6)) /*Changes kb typeamatic rate*/
				{
					video.prints(system.StatusBar, "\n Setting keyboard typeamatic rate...");
					setKBRate(Window, par[1]);
					video.putc(Window, '\n');
				}
				else if(!memcmp(buffer, "cls", 3)) /*clears screen and moves cursor to top*/
				{
					video.cls(Window);
					video.gotoxy(Window, 0, (video.scrheight - 2));
				}
				else if(!memcmp(buffer, "restart", 7)) /*restarts pc after user acknoledgement*/
				{
					video.prints(system.StatusBar, "\n Restarting...");
					return restartIt(Window, par[1], par[2]);
				}
				else if(!memcmp(buffer, "end", 3)) /*halts cpu. Displays shutdown screen*/
				{
					video.hidecursor(true);
					video.prints(system.StatusBar, "\n System halted.");
					return restartIt(Window, buffer, par[0]);
				}
				else if(!memcmp(buffer, "help", 4)) /*displays help*/
				{
					video.prints(Window, "    -Console Command Help-\n    ú    ver:      Displays OS version\n    ú    mem:      Displays memory statistics\n    ú    cls:      Clear's the screen\n    ú    time:     Displays system time. Use 'time 24' for 24 hour time\n    ú    date:     Displays system date. Use 'date long' for long date\n    ú    kbrate:   Used to change the typeamatic rate and delay of the keyboard\n    ú    restart:  Restarts the computer\n    ú    end:      Ends your console session\n");
				}
				else
				{
					if(control > 0) /*Warning about unknown command*/
					{
						video.prints(Window, "    Unknown command.\n    For help with commands and their functions type 'help'.\n");
					}
				}

				video.prints(Window, "> "); /*resets buffer and input line*/
				control = 0;
				bs = 0;
				video.prints(system.StatusBar, "\n For help with commands type 'help'.");
			}
			else if(c == 8)
			{
				if(bs > 0) /*code that makes backspacing work*/
				{
					control--;
					bs--;
					video.putc(Window, c);
				}
			}
			else
			{
				if(!(c < ' '))
				{
					buffer[control] = c;
					if(control < (BUFFYSIZE - 1)) /*this stops buffer overflows*/
					{ 
						control++;
						bs++;
					}
					else
					{
						beep(3, HIA);	/*Beeps at the end of the buffer*/
						video.putc(Window, 8);	/*removes the last char typed and*/
					}
					video.putc(Window, c);		/*Adds this char to the screen*/
				}
			}
		}
}


void Console::strsplit(unsigned char *str1, unsigned char *p1,unsigned char *p2,unsigned char *p3,unsigned char *p4,unsigned char *p5)
{						/*Splits commandline into command and parameters*/
	int pnr = 1;

	while(*str1 != '\0')
	{
		while(*str1 != ' ')
		{
			if(*str1 == '\0')
			{ 
				*p1 = '\0';*p2 = '\0';*p3 = '\0';*p4 = '\0';*p5 = '\0';
				break;
			}
			if(pnr == 1){*p1 = *str1;*p1++;*str1++;}
			if(pnr == 2){*p2 = *str1;*p2++;*str1++;}
			if(pnr == 3){*p3 = *str1;*p3++;*str1++;}
			if(pnr == 4){*p4 = *str1;*p4++;*str1++;}
			if(pnr == 5){*p5 = *str1;*p5++;*str1++;}
			if(pnr >  5){*p1 = 0;*p2 = 0;*p3 = 0;*p4 = 0;*p5 = 0;break;}
		}
		pnr++;
		*str1++;
	}
}


int Console::restartIt(int Window, unsigned char *p1, unsigned char *p2)
{				/*Selects the restart type for the user*/

	video.setcolour(Window, TLIGHTCYAN | BBLUE);

	if(!memcmp(p1, "end", 3))
	{
		video.cls(Window);
		video.gotoxy(Window, 0, (video.scrheight / 2 - 2));
		video.centerText(Window, "It is now safe to turn off your computer.");
		return 0;
	}
	else if(!memcmp(p1, "now", 3))
	{
		return 1;
	}

	video.prints(Window, "\nPress any key to restart the computer...");
	keyboard.getch();

	return 1;
}

void Console::setKBRate(int Window, unsigned char *rate) /*Sets keyboard typeamatic rate*/
{
	switch(*rate)
	{
	case 'd':
		keyboard.SetRate(KDefault);
		break;

	case 'h':
		keyboard.SetRate(K25Delay | K0Rate);
		break;

	case 'm':
		keyboard.SetRate(K50Delay | K4Rate);
		break;

	case 'l':
		keyboard.SetRate(K75Delay | KL0Rate);
		break;

	default:
		system.assert(Window, 1,"Invalid typeamatic rate. Keyboard rate not set.");
		return;
	}
	video.prints(Window, "    Keyboard typeamatic rate set.");
}

void Console::dispTimeDate(int Window, unsigned char *display, unsigned char *type)
{ /*Displays system date or time*/

	if(*display == 't') /*Display time:*/
	{
		unsigned int hour;
		bool PM;
		bool Type24;

		time(sysTime);

		if(*type == '2')
		{
			Type24 = true;
			hour = sysTime.hour;
		}
		else
		{
			Type24 = false;
			if(sysTime.hour > 12)
			{
				hour = sysTime.hour - 12;
				PM = true;
			}
			else
			{
				hour = sysTime.hour;
				PM = false;
			}
		}

		ntostr(hour, str);
		if(hour < 10)
		{
			video.putc(Window, '0');
		}
		video.prints(Window, str);
		video.putc(Window, ':');
		ntostr(sysTime.minute, str);
		if(sysTime.minute < 10)
		{
			video.putc(Window, '0');
		}
		video.prints(Window, str);

		if(Type24 == false)
		{
			if(PM == true)
			{
				video.prints(Window, " PM");
			}
			else
			{
				video.prints(Window, " AM");
			}
		}
	}
	else
	{
		date(sysDate); /*Display date:*/

		if(*type == 'l')
		{

			switch(sysDate.day)
			{
			case 1:
				video.prints(Window, "Monday, \0");
				break;
			case 2:
				video.prints(Window, "Tuesday, \0");
				break;
			case 3:
				video.prints(Window, "Wednesday, \0");
				break;
			case 4:
				video.prints(Window, "Thursday, \0");
				break;
			case 5:
				video.prints(Window, "Friday, \0");
				break;
			case 6:
				video.prints(Window, "Saturday, \0");
				break;
			case 7:
				video.prints(Window, "Sunday, \0");
			}
			ntostr(sysDate.date, str);
			video.prints(Window, str);
			switch(sysDate.month)
			{
			case 1:
				video.prints(Window, " January \0");
				break;
			case 2:
				video.prints(Window, " Febuary \0");
				break;
			case 3:
				video.prints(Window, " March \0");
				break;
			case 4:
				video.prints(Window, " April \0");
				break;
			case 5:
				video.prints(Window, " May \0");
				break;
			case 6:
				video.prints(Window, " June \0");
				break;
			case 7:
				video.prints(Window, " July \0");
				break;
			case 8:
				video.prints(Window, " August \0");
				break;
			case 9:
				video.prints(Window, " September \0");
				break;
			case 10:
				video.prints(Window, " October \0");
				break;
			case 11:
				video.prints(Window, " November \0");
				break;
			case 12:
				video.prints(Window, " December \0");
			}
		}
		else
		{
			ntostr(sysDate.date, str);
			if(sysDate.date < 10)
			{
				video.putc(Window, '0');
			}
			video.prints(Window, str);
			video.putc(Window, '/');
			ntostr(sysDate.month, str);
			if(sysDate.month < 10)
			{
				video.putc(Window, '0');
			}
			video.prints(Window, str);
			video.putc(Window, '/');
		}

		ntostr(sysDate.year, str);
		video.prints(Window, str);
	}
}
