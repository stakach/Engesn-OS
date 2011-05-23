/*
	PConsole.c
	---------
	(c) Engesn Software Corp.
    Text input and command interpreter for Engesn\OS1 Ver 0.07

*/

#include "Pkernel.h"

#define BUFFSIZE 256 /*amount of characters acknowledged on the screen*/

void strsplit(unsigned char *str1, unsigned char *p1,unsigned char *p2,unsigned char *p3,unsigned char *p4,unsigned char *p5);
int restartIt(int Window, unsigned char *p1, unsigned char *p2);
void setKBRate(int Window, unsigned char *rate);
void dispTimeDate(int Window, unsigned char *display, unsigned char *type);

unsigned short major = 0;
unsigned short minor = 9;
unsigned short build = 45;
char str[20];

unsigned int startConsole(int Window) /*Engesn temporary input console*/
{
	size_t control = 0;
	size_t bs = 0;
	uchar c;
	uchar buffer[BUFFSIZE];
	uchar par[5][BUFFSIZE];

	buffer[control] = '\0';
	prints(Window, "\n> ");
	prints(StatusBar, "\n For help with commands type 'help'.");
	kbFlushBuffer();

		while(1)
		{
			cls(StatusBar + 1);
			dispTimeDate(StatusBar + 1, "t\0", '\0');

			c = getch();
			if(c == '\r')
			{
				ushort i;
				tty_putc(Window, '\n');
				buffer[control] = '\0';
				buffer[control + 1] = '\0';

				strsplit(buffer, par[0], par[1], par[2], par[3], par[4]);

				if(!memcmp(buffer, "ver", 3)) /*Displays version number*/
				{
					prints(Window, "\n    EngesnOS/1 Ver: ");
					ntostr(major, str);
					prints(Window, str);
					tty_putc(Window, '.');
					if(minor < 10)
					{
						tty_putc(Window, '0');
					}
					ntostr(minor, str);
					prints(Window, str);
					if(!memcmp(par[1], "build", 5)) /*Displays build number*/
					{
						ntostr(build, str);
						prints(Window, "\n             Build: ");
						prints(Window, str);
					}
					prints(Window, "\n\n");
				}
				else if(!memcmp(buffer, "mem", 3)) /*Displays memory statistics*/
				{
					prints(Window, "     Low Mem: ");
					ntostr(convmem_size, str);
					prints(Window, str);
					prints(Window, "KB");
					prints(Window, "\n    High Mem: ");
					ntostr(extmem_size, str);
					prints(Window, str);
					prints(Window, "KB");
					tty_putc(Window, '\n');
				}
				else if(!memcmp(buffer, "play", 4)) /*Plays a user defined tune*/
				{
					prints(StatusBar, "\n Now playing...");
					playString(par[1]);
					tty_putc(Window, '\n');
				}
				else if(!memcmp(buffer, "time", 4)) /*Displays system time*/
				{
					prints(Window, "    ");
					dispTimeDate(Window, par[0], par[1]);
					tty_putc(Window, '\n');
				}
				else if(!memcmp(buffer, "date", 4)) /*Displays system Date*/
				{
					prints(Window, "    ");
					dispTimeDate(Window, par[0], par[1]);
					tty_putc(Window, '\n');
				}
				else if(!memcmp(buffer, "kbrate", 6)) /*Changes kb typeamatic rate*/
				{
					prints(StatusBar, "\n Setting keyboard typeamatic rate...");
					setKBRate(Window, par[1]);
					tty_putc(Window, '\n');
				}
				else if(!memcmp(buffer, "cls", 3)) /*clears screen and moves cursor to top*/
				{
					cls(Window);
					gotoxy(Window, 0, (scrheight - 2));
				}
				else if(!memcmp(buffer, "screensav", 9)) /*Turns screen off and waits for user input*/
				{
					screenStatus(FALSE);
					getch();
					screenStatus(TRUE);
				}
				else if(!memcmp(buffer, "restart", 7)) /*restarts pc after user acknoledgement*/
				{
					prints(StatusBar, "\n Restarting...");
					return restartIt(Window, par[1], par[2]);
				}
				else if(!memcmp(buffer, "end", 3)) /*halts cpu. Displays shutdown screen*/
				{
					hidecursor(TRUE);
					prints(StatusBar, "\n System halted.");
					return restartIt(Window, buffer, par[0]);
				}
				else if(!memcmp(buffer, "help", 4)) /*displays help*/
				{
					prints(Window, "    -Console Command Help-\n    ú    ver:      Displays OS version\n    ú    mem:      Displays memory statistics\n    ú    cls:      Clear's the screen\n    ú    screensav:Turns off the monitor and waits for user input\n    ú    time:     Displays system time. Use 'time 24' for 24 hour time\n    ú    date:     Displays system date. Use 'date long' for long date\n    ú    kbrate:   Used to change the typeamatic rate and delay of the keyboard\n    ú    play:     Plays a user defined tune, eg. 'play ABCDE;'\n    ú    restart:  Restarts the computer\n    ú    end:      Ends your console session\n");
				}
				else
				{
					if(control > 0) /*Warning about unknown command*/
					{
						prints(Window, "    Unknown command.\n    For help with commands and their functions type 'help'.\n");
					}
				}

				prints(Window, "> "); /*resets buffer and input line*/
				control = 0;
				bs = 0;
				prints(StatusBar, "\n For help with commands type 'help'.");
			}
			else if(c == 8)
			{
				if(bs > 0) /*code that makes backspacing work*/
				{
					control--;
					bs--;
					tty_putc(Window, c);
				}
			}
			else
			{
				if(!(c < ' '))
				{
					buffer[control] = c;
					if(control < (BUFFSIZE - 1)) /*this stops buffer overflows*/
					{ 
						control++;
						bs++;
					}
					else
					{
						beep(3, HIA);	/*Beeps at the end of the buffer*/
						tty_putc(Window, 8);	/*removes the last char typed and*/
					}
					tty_putc(Window, c);		/*Adds this char to the screen*/
				}
			}
		}
}


void strsplit(unsigned char *str1, unsigned char *p1,unsigned char *p2,unsigned char *p3,unsigned char *p4,unsigned char *p5)
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


int restartIt(int Window, unsigned char *p1, unsigned char *p2)
{				/*Selects the restart type for the user*/
	int par = 2; 

	tty_setcolour(Window, TLIGHTCYAN | BBLUE);

	if(!memcmp(p1, "end", 3))
	{
		cls(Window);
		gotoxy(Window, 0, (scrheight / 2 - 2));
		centerText(Window, "It is now safe to turn off your computer.");
		return 0;
	}

	if(!memcmp(p1, "cold", 4))
	{
		par = 1;
		if(!memcmp(p2, "now", 3)) /*If 'now', then don't wait for user input*/
		{
			return par;
		}
	}
	else if(!memcmp(p1, "now", 3))
	{
		if(!memcmp(p2, "cold", 4))
		{
			par = 1;
		}
		return par;
	}

	prints(Window, "\nPress any key to restart the computer...");
	getch();

	return par;
}

void setKBRate(int Window, unsigned char *rate) /*Sets keyboard typeamatic rate*/
{
	switch(*rate)
	{
	case 'd':
		kbSetRate(KDefault);
		break;

	case 'h':
		kbSetRate(K25Delay | K0Rate);
		break;

	case 'm':
		kbSetRate(K50Delay | K4Rate);
		break;

	case 'l':
		kbSetRate(K75Delay | KL0Rate);
		break;

	default:
		assert(Window, 1,"Invalid typeamatic rate. Keyboard rate not set.");
		return;
	}
	prints(Window, "    Keyboard typeamatic rate set.");
}

void dispTimeDate(int Window, unsigned char *display, unsigned char *type)
{ /*Displays system date or time*/

	if(*display == 't') /*Display time:*/
	{
		unsigned int hour;
		boolean PM;
		boolean Type24;

		get_time(&sysTime);

		if(*type == '2')
		{
			Type24 = TRUE;
			hour = sysTime.hour;
		}
		else
		{
			Type24 = FALSE;
			if(sysTime.hour > 12)
			{
				hour = sysTime.hour - 12;
				PM = TRUE;
			}
			else
			{
				hour = sysTime.hour;
				PM = FALSE;
			}
		}

		ntostr(hour, str);
		if(hour < 10)
		{
			tty_putc(Window, '0');
		}
		prints(Window, str);
		tty_putc(Window, ':');
		ntostr(sysTime.minute, str);
		if(sysTime.minute < 10)
		{
			tty_putc(Window, '0');
		}
		prints(Window, str);

		if(Type24 == FALSE)
		{
			if(PM == TRUE)
			{
				prints(Window, " PM");
			}
			else
			{
				prints(Window, " AM");
			}
		}
	}
	else
	{
		get_date(&sysDate); /*Display date:*/

		if(*type == 'l')
		{

			switch(sysDate.day)
			{
			case 1:
				prints(Window, "Monday, \0");
				break;
			case 2:
				prints(Window, "Tuesday, \0");
				break;
			case 3:
				prints(Window, "Wednesday, \0");
				break;
			case 4:
				prints(Window, "Thursday, \0");
				break;
			case 5:
				prints(Window, "Friday, \0");
				break;
			case 6:
				prints(Window, "Saturday, \0");
				break;
			case 7:
				prints(Window, "Sunday, \0");
			}
			ntostr(sysDate.date, str);
			prints(Window, str);
			switch(sysDate.month)
			{
			case 1:
				prints(Window, " January \0");
				break;
			case 2:
				prints(Window, " Febuary \0");
				break;
			case 3:
				prints(Window, " March \0");
				break;
			case 4:
				prints(Window, " April \0");
				break;
			case 5:
				prints(Window, " May \0");
				break;
			case 6:
				prints(Window, " June \0");
				break;
			case 7:
				prints(Window, " July \0");
				break;
			case 8:
				prints(Window, " August \0");
				break;
			case 9:
				prints(Window, " September \0");
				break;
			case 10:
				prints(Window, " October \0");
				break;
			case 11:
				prints(Window, " November \0");
				break;
			case 12:
				prints(Window, " December \0");
			}
		}
		else
		{
			ntostr(sysDate.date, str);
			if(sysDate.date < 10)
			{
				tty_putc(Window, '0');
			}
			prints(Window, str);
			tty_putc(Window, '/');
			ntostr(sysDate.month, str);
			if(sysDate.month < 10)
			{
				tty_putc(Window, '0');
			}
			prints(Window, str);
			tty_putc(Window, '/');
		}

		ntostr(sysDate.year, str);
		prints(Window, str);
	}
}
