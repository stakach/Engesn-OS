#ifndef _dispIO_H_
#define _dispIO_H_

/*The values for the different text colours  */
/*-------------------------------------------*/
#define TBLACK		0
#define TBLUE		1
#define TGREEN		2
#define TCYAN		3
#define TRED		4
#define TMAGNETA	5
#define TBROWN		6
#define TWHITE		7
#define TGRAY		8
#define TLIGHTBLUE	9
#define TLIGHTGREEN	10
#define TLIGHTCYAN	11
#define TPINK		12
#define	TYELLOW		14
#define TRUEWHITE	15
/*-------------------------------------------*/


/*The values for the different background colours*/
/*-----------------------------------------------*/
#define BBLACK		0
#define BBLUE		16
#define BGREEN		32
#define BCYAN		48
#define BRED		64
#define BMAGNETA	80
#define BBROWN		96
#define BWHITE		112
/*-----------------------------------------------*/


/*Sets the text to blink*/
/*----------------------*/
#define BLINK	128
/*----------------------*/


void prints(int wno, char *s); /*prints text at the cursor location*/
void centerText(int wno, char *s); /*centers text on the screen*/
void printsRight(int wno, char *s); /*prints text with right justify*/
void cls(int wno); /*clears the screen*/
void tty_putc(int wno, char c); /*Places a character on the screen*/
void tty_setcolour(int wno, unsigned char colour); /*sets text background and text colour*/

void tty_setcursor(int wno, unsigned x, unsigned y); /*moves the cursor*/
void hidecursor(boolean curs); /*Hides the cursor for all windows*/
void gotoxy(int wno, unsigned x, unsigned y); /*moves cursor to xy location*/
void enablecursor(int wno, boolean status); /*Enables/Disables the cursor for display in a certain window*/

int  new_window(int x1, int y1, int x2,int y2); /*Creats a new text window*/
void disablewindow(int wno, boolean stat); /*Enables / Disables an already created window*/
void tty_setup(int width, int height); /*allows re-definition of screen width for mode change*/


#define MAXWINDOW 20        /* Number of user windows at a time */

typedef struct
{
	 unsigned int height, width, xpos, ypos, xhome, yhome;
	 boolean status, movcursor;
	 unsigned char colour;
	 unsigned int far *buf;
} wnoData;         /* for creating task windows */


extern int scrwidth, scrheight;	/* terminal dimensions (video bios mode 3) */

#endif
