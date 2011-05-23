#ifndef _kerndefs_H_
#define _kerndefs_H_

/*Time saving Definitions*/
#ifdef size_t
#undef size_t
#endif
#ifndef NULL
#define NULL 0
#endif

#define LOWBYTE(v)		((unsigned char) (v)) /*for obtaining low and high bytes*/
#define HIGHBYTE(v)		((unsigned char) (((unsigned int) (v)) >> 8))

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef ushort		size_t;
typedef uchar		byte ;
typedef byte        BYTE ;
typedef	int			int16 ;
typedef long		int32 ;
typedef ushort		word ;
typedef word        WORD ;
typedef ulong		dword ;
typedef dword           DWORD ;
typedef	unsigned int	UINT ;


#define	BIT(x)	(1<<(x))

typedef void (*voidfun)();
typedef int (*intfun)();


#endif
