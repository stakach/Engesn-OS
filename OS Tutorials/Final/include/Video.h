#ifndef VIDEO_H
#define VIDEO_H

#include "Modes.h"

//The different colours that can be set. Only the first 7 are relevent for background
enum Colours {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, WHITE, DARKGRAY,
	BRIGHTBLUE, BRIGHTGREEN, BRIGHTCYAN, PINK, BRIGHTMAGENTA, YELLOW, BRIGHTWHITE};

class Video : public modes	//Video class 
{
	public:
		Video() ;
		~Video() ;
		void clear() ;
		void write(char *cp) ;
		void put(char c) ;
		void gotoxy(unsigned x, unsigned y) ;
		void SetColour(Colours Text, Colours Back, bool blink);
		unsigned char GetBackColour();
		unsigned char GetTextColour();
		void SetBackColour(Colours col);
		void SetTextColour(Colours col);
		void cursorVisible(bool status);

	private:
		void scrollup() ;
		void setcursor();

		unsigned int ypos ;				//offset, used like a y cord
		unsigned int xpos ;				//position, used like x cord
		unsigned char colour ;			//Text colours
		unsigned char text ;
		unsigned char back ;
};


#endif

