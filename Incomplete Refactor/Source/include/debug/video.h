#ifndef __VIDOUT_H__
#define __VIDOUT_H__


#include <def.h>
#include <hal.h>


//
// The different colours that can be set. Only the first 7 are relevent for background.
//

enum COLOURS {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, WHITE, DARKGRAY,
	BRIGHTBLUE, BRIGHTGREEN, BRIGHTCYAN, PINK, BRIGHTMAGENTA, YELLOW, BRIGHTWHITE};


namespace DEBUG
{


	class VIDOUT: public DEBUG_CONSOLE
	{
		public:
			virtual VOID Clear();
			virtual VOID Write(CHAR *cp);
			virtual VOID Write(ULONG Num, UINT Base);
			virtual VOID Put(const CHAR &c);
			virtual VOID GotoXY(UINT x, UINT y);
			VOID SetColour(COLOURS Text, COLOURS Back, BOOLEAN Blink);
			VOID SetBackColour(COLOURS Col);
			VOID SetTextColour(COLOURS Col);
			VOID CursorVisible(BOOLEAN Status);
			VOID SetCursorType(BYTE Bottom, BYTE Top);
			
			static VIDOUT Vidout;

		private:
			VIDOUT();
			VOID ScrollUp();
			VOID SetCursor();	

			UINT YPos;				//offset, used like a y cord
			UINT XPos;				//position, used like x cord
			UCHAR Colour;			//Text colours
			UCHAR Text;
			UCHAR Back;
			USHORT *VideoMem;		//pointer to video memory
			USHORT CrtcMem;
			UINT ScrHeight;
			UINT ScrWidth;
	};


}


#endif
