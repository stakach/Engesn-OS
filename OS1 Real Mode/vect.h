#ifndef __vect_h__
#define __vect_h__

#include "asmf.h"

#define	TIMER_VECT	8


void setvect(void interrupt (far *fp)(),int intno);
void interrupt (far * getvect(int intno))();


#endif

