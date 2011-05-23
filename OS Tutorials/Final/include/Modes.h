#ifndef __MODES_H__
#define __MODES_H__

typedef struct
{
	unsigned short	first, last;
	unsigned char	height;
	unsigned char	*bitmaps;
} Font;

enum Modes {m8x25, m8x50, m9x60};

class modes
{
	public:
		modes();
		void setMode(Modes mode);

	private:
		void set_font(Font *font);
		void outsw(unsigned short adr, unsigned short *data, unsigned count);
		void write_regs(unsigned char *regs);

	protected:
		unsigned short *videomem;		//pointer to video memory
		unsigned short crtc_mem;
		unsigned int scrHeight;
		unsigned int scrWidth;
};


#endif

