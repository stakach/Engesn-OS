#ifndef _video_H_
#define _video_H_

#include "kerndefs.h"

void vidSetup();
void screenStatus(boolean Enabled);

extern boolean screenEnabled; /*Is the screen enabled? Should not be changed*/
extern int far * screen_mem;	/*0xb8000 for colour, 0xb0000 for mono*/
extern unsigned short crtc_mem;	/*0x3D4 for colour, 0x3B4 for monochrome*/

#endif

