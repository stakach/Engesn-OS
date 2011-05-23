#ifndef __VIDEO_H__
#define __VIDEO_H__		// So we don't get multiple definitions of Video.


class Video
{
	public:
		Video();
		void clear();
		void write(char *cp);
		void put(char c);

	private:
		unsigned short *videomem;	// Pointer to video memory.
		unsigned int y;				// Offset, used like a y cord.
		unsigned int x;				// Position, used like x cord.
};


#endif
