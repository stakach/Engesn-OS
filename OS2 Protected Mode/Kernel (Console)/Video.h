#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "kerndefs.h"
#include "asmf.h"
#include "string.h"



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



class Video
{
	public:
		Video();
		~Video();
		int new_window(int x1, int y1, int x2,int y2);
		void setcolour(int wno, unsigned char colour);
		void scrollup(int wno);
		void putc(int wno, char c);
		void cls(int wno);
		void prints(int wno, char* str);
		void centerText(int wno, char *s);
		void printsRight(int wno, char *s);
		void gotoxy(int wno, unsigned x, unsigned y);
		void enablecursor(int wno, bool curs);
		void hidecursor(bool curs);
		void disablewindow(int wno, bool stat);

		unsigned int scrwidth, scrheight;


	private:
		void setcursor(int wno, unsigned x, unsigned y);

		unsigned short *videomem; //pointer to video memory
		unsigned short crtc_mem;
		bool CursorHidden;     /*Cursor hidden?*/

		typedef struct
		{
			 unsigned int height, width, xpos, ypos, xhome, yhome;
			 bool status, movcursor;
			 unsigned char colour;
			 unsigned short *buf;
		} wnoData;         /* for creating task windows */
		#define MAXWINDOW 5  /* Number of user windows at a time */

		wnoData window_info[MAXWINDOW];
};

#endif 
