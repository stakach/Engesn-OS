#include "speaker.h"

void speakerOn(unsigned int pitch)	/*Enables the speaker and sets frequency*/
{
	outportb(0x43, 0xb6);
	outportb(0x42, LOWBYTE(pitch));
	outportb(0x42, HIGHBYTE(pitch));
	outportb(0x61, (inportb(0x61) | 3));
}

void speakerOff()					/*Disables the speaker*/
{
	outportb(0x61, (inportb(0x61) & 0xfc));
}

void beep(unsigned int delay, unsigned int pitch) /*turns on the speaker for a defined period of time*/
{
	speakerOn(pitch);
	Delay(delay);
	speakerOff();
}

void playString(unsigned char *notes)	/*parses a string and determines which notes to play*/
{
	unsigned short int octave = 1; /*1 to  2  valid*/
	unsigned short int delay  = 3; /*3 to 12  valid*/
	unsigned short int pause  = 3; /*3 to 12  valid*/
	unsigned short int note   = 1; /*1 to  7  valid*/
	boolean playNote = FALSE;
	boolean sharp = FALSE;

	while (!((*(notes) == '\0') || (*(notes + 1) == '\0'))) /*Tests to make sure there are 2 chars left*/
	{
		switch (*(notes)) /*parses the input based on the current char(s)*/
		{
		case 'o':		/*Sets octave*/
			*(notes++);
			if ((*(notes) <= 50) && (*(notes) >= 49))
			{
				octave = *(notes) - 48;
			}
			break;

		case 'w':		/*Sets the width/length of the note*/
			*notes++;
			if ((*(notes) <= 57) && (*(notes) > 48))
			{
				delay = *(notes) - 45;
			}
			else
			{
				delay = 3;
			}
			break;

		case 'p':		/*Causes a pause in the playing of the notes*/
			*notes++;
			if ((*(notes) <= 57) && (*(notes) > 48))
			{
				pause = *(notes) - 45;
			}
			else
			{
				pause = 3;
			}
			Delay(pause);
			break;

		default:		/*Determines what note to play if input is valid*/
			if ((*(notes) <= 71) && (*(notes) >= 65))
			{
				note = *(notes) - 64; /*converts ASCII code to number*/
				playNote = TRUE;
				if (*(notes + 1) == '#') /*is the note sharp?*/
				{
					*(notes++);
					sharp = TRUE;
				}
			}
			break;
		}
		*(notes++);

		if (playNote == TRUE) /*Plays the note determined by the parser*/
		{
			switch (octave)
			{
			case 1:
				switch (note)
				{
				case 1:
					if (sharp == FALSE)
					{
						beep(delay, LOA);
					}
					else
					{
						beep(delay, LOAS);
					}
					break;

				case 2:
					beep(delay, LOB);
					break;

				case 3:
					if (sharp == FALSE)
					{
						beep(delay, MIDC);
					}
					else
					{
						beep(delay, HICS);
					}
					break;

				case 4:
					if (sharp == FALSE)
					{
						beep(delay, HID);
					}
					else
					{
						beep(delay, HIDS);
					}
					break;

				case 5:
					beep(delay, HIE);
					break;

				case 6:
					if (sharp == FALSE)
					{
						beep(delay, HIF);
					}
					else
					{
						beep(delay, HIFS);
					}
					break;

				case 7:
					if (sharp == FALSE)
					{
						beep(delay, HIG);
					}
					else
					{
						beep(delay, HIGS);
					}
					break;

				default:
					break;
				}
				break;

			case 2:
				switch (note)
				{
				case 1:
					if (sharp == FALSE)
					{
						beep(delay, HIA);
					}
					else
					{
						beep(delay, HIAS);
					}
					break;

				case 2:
					beep(delay, HIB);
					break;

				case 3:
					if (sharp == FALSE)
					{
						beep(delay, UPPERC);
					}
					else
					{
						beep(delay, UPPERCS);
					}
					break;

				case 4:
					if (sharp == FALSE)
					{
						beep(delay, UPPERD);
					}
					else
					{
						beep(delay, UPPERDS);
					}
					break;

				case 5:
					beep(delay, UPPERE);
					break;

				case 6:
					if (sharp == FALSE)
					{
						beep(delay, UPPERF);
					}
					else
					{
						beep(delay, UPPERFS);
					}
					break;

				case 7:
					if (sharp == FALSE)
					{
						beep(delay, UPPERG);
					}
					else
					{
						beep(delay, UPPERGS);
					}
					break;

				default:
					break;
				}
				break;

			default:
				break;
			}
		}

		playNote = FALSE;
		sharp = FALSE;
	}
}