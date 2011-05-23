#ifndef __MATHS_H__
#define __MATHS_H__

#define bcd2dec(bcd) ((((bcd) >> 4) * 0xA) + ((bcd) & 0xf))	//binary coded decimal to binary converter
#define	BIT(x)	(1<<(x))


#endif

