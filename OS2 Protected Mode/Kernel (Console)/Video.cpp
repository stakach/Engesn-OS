#include "Video.h" 


Video::Video()
{
	char c = (*(ushort*)0x410&0x30);/*Detects video card type, vga or mono*/

	/* C can be 0x00 or 0x20 for colour, 0x30 for mono*/
	if(c == 0x30)
	{
		videomem = (unsigned short*) 0xb0000;
		crtc_mem   = 0x3B4;	/* mono*/
	}
	else
	{
		videomem = (unsigned short*) 0xb8000;
		crtc_mem   = 0x3D4;	/* colour*/
	}

	scrwidth = 80;
	scrheight = 50;
	CursorHidden = false;
}



Video::~Video() {}



int Video::new_window(int x1, int y1, int x2,int y2)
{
/*
     this function will open a window with given corner points.
     And it will returns a window token to this window.
*/
  static int wno=0;
     if((wno+1) >= MAXWINDOW)return(-1);
     window_info[wno].buf = (unsigned short *)(videomem + y1 * scrwidth + x1);
     window_info[wno].height    = y2-y1 + 1;
     window_info[wno].width     = x2-x1 + 1;
     window_info[wno].status    = true;
	 window_info[wno].movcursor = false;
     window_info[wno].xpos      = 0;
	 window_info[wno].ypos      = 0;
	 window_info[wno].xhome		= x1;
	 window_info[wno].yhome		= y1;
	 window_info[wno].colour    = 0x7;
     wno++;
     return(wno-1);
}



void Video::setcursor(int wno, unsigned x, unsigned y)
{
	unsigned short crtc_adr = 0x3D4; /*0x3B4 for monochrome */
    unsigned short offset;

	if ((window_info[wno].movcursor == true) && (CursorHidden == false))
	{
		offset = (x + window_info[wno].xhome) + (y + window_info[wno].yhome) * scrwidth;  /* 80 characters per line */
		outportb(crtc_mem    , 14);     /* MSB of offset to CRTC reg 14 */
		outportb(crtc_mem + 1, HIGHBYTE(offset));
		outportb(crtc_mem    , 15);     /* LSB of offset to CRTC reg 15 */
		outportb(crtc_mem + 1, LOWBYTE(offset));
	}
}


void Video::setcolour(int wno, unsigned char colour)
{
	window_info[wno].colour = colour;       /* sets text and background colour */
}


void Video::scrollup(int wno) /* scroll the screen up one line */
{
	int t;
	int x;

	disable();

	for(t = 0; t < window_info[wno].width * (window_info[wno].height - 1); t++)	/* scroll every line up*/
	{
		x = t + window_info[wno].width;
		*(window_info[wno].buf + ((t / window_info[wno].width) * scrwidth) + ((t % window_info[wno].width))) = *(window_info[wno].buf + ((x / window_info[wno].width) * scrwidth) + ((x % window_info[wno].width)));
	}
	for(; t < window_info[wno].width * window_info[wno].height; t++)			/* clear the bottom line */
	{
		*(window_info[wno].buf + ((t / window_info[wno].width) * scrwidth) + ((t % window_info[wno].width))) = ' ' | (window_info[wno].colour << 8);
	}

	enable();
}



/* Screen Output functions -----------------------------------------------------------------*/

void Video::putc(int wno, char c) /* print one character on the screen */
{
	int t;
	switch(c)
	{
	case '\r':                         /* -> carriage return */
		window_info[wno].xpos = 0;
		break;

	case '\n':                         /* -> newline (with implicit cr) */
		window_info[wno].xpos = 0;
		window_info[wno].ypos++;
		break;

	case 8:                            /* -> backspace */
		t = window_info[wno].xpos + window_info[wno].ypos * window_info[wno].width;     /* get linear address */
		if(t > 0) t--;      
									   /* if not in home position, step back */
		if(window_info[wno].xpos > 0)
		{
			window_info[wno].xpos--;
		}
		else if(window_info[wno].ypos > 0)
		{
			window_info[wno].ypos--;
			window_info[wno].xpos = window_info[wno].width - 1;
		}

		*(window_info[wno].buf + ((t / window_info[wno].width) * scrwidth) + ((t % window_info[wno].width))) = ' ' | (window_info[wno].colour << 8); /* put space under the cursor */
		break;

	default:                           /* -> all other characters */
		if(c < ' ') break;             /* ignore non printable ascii chars */
		t = window_info[wno].xpos + window_info[wno].ypos * window_info[wno].width;
		*(window_info[wno].buf + ((t / window_info[wno].width) * scrwidth) + ((t % window_info[wno].width))) = c | (window_info[wno].colour << 8);
		window_info[wno].xpos++;                       /* step cursor one character */
		if(window_info[wno].xpos == window_info[wno].width)             /* to next line if required */
		{
			window_info[wno].xpos = 0;
			window_info[wno].ypos++;
		}
		break;
	}

	if(window_info[wno].ypos == window_info[wno].height)                  /* the cursor moved off of the screen? */
	{
		scrollup(wno);                 /* scroll the screen up */
		window_info[wno].ypos--;           /* and move the cursor back */
	}
										/* and finally, set the cursor */
	if (window_info[wno].movcursor == true)
	{
		setcursor(wno, window_info[wno].xpos, window_info[wno].ypos);
	}
}


void Video::cls(int wno) /* clear the screen by filling it with spaces */
{
	int t;

	for(t = 0; t < window_info[wno].width * window_info[wno].height; t++)		/* clear the screen */
	{
		*(window_info[wno].buf + ((t / window_info[wno].width) * scrwidth) + ((t % window_info[wno].width))) = ' ' | (window_info[wno].colour << 8);
	}

	window_info[wno].xpos = 0; 
	window_info[wno].ypos = 0;					/* set cursor to home location */
	
	if (window_info[wno].movcursor == true)
	{
		setcursor(wno, 0, 0);						/* call the hw. cursor setup */
	}
}


void Video::prints(int wno, char* str)
{
	while((*str) != 0) putc(wno, *(str++));
}


void Video::centerText(int wno, char *s) /*Outputs text to the center of the screen*/
{
	unsigned int blanks;
	unsigned int counter = 0;
	blanks = ((window_info[wno].width - strlen(s)) / 2);
	while(counter < blanks)
	{
		putc(wno, ' ');
		counter++;
	}
	prints(wno, s);
	putc(wno, '\n');
}


void Video::printsRight(int wno, char *s) /*Outputs text to the right of the screen*/
{
	unsigned int counter = 0;
	unsigned int spaces;
	spaces = (window_info[wno].width - strlen(s));
	while(counter < spaces)
	{
		putc(wno, ' ');
		counter++;
	}
	prints(wno, s);
	putc(wno, '\n');
}



/* some misc. functions -----------------------------------------------------------------*/

void Video::gotoxy(int wno, unsigned x, unsigned y)
{
	window_info[wno].xpos = x;
	window_info[wno].ypos = y;			/* set cursor to home location */
	setcursor(wno, x, y);			/* call the hw. cursor setup */
}


void Video::enablecursor(int wno, bool curs) /* Allows window to control the cursor*/
{
	window_info[wno].movcursor = curs;
}


void Video::hidecursor(bool curs) /* Hides cursor from the screen */
{
	unsigned short offset;
	CursorHidden = curs;
	if (curs == true)
	{
		offset = scrheight * scrwidth;         /* This really only moves the cursor one line off the screen */
		outportb(crtc_mem + 0, 14);     /* MSB of offset to CRTC reg 14 */
		outportb(crtc_mem + 1, HIGHBYTE(offset));
		outportb(crtc_mem + 0, 15);     /* LSB of offset to CRTC reg 15 */
		outportb(crtc_mem + 1, LOWBYTE(offset));
	}

}


void Video::disablewindow(int wno, bool stat) /* Free's the memory related to this window */
{
	window_info[wno].status = stat;
}

