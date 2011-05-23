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

#define GetSeg(obj)		(unsigned)((unsigned long)(((void FAR *)(obj))) >> 16)
#define GetOfs(obj)		(unsigned)((unsigned long)(((void FAR *)(obj))) & 0xffff)
#define FP_OFF(fp)	((unsigned)(fp))
#define FP_SEG(fp)	((unsigned)((unsigned long)(fp) >> 16))
#define MK_FP(seg,ofs)	((void far *) \
			   (((unsigned long)(seg) << 16) | (unsigned)(ofs)))

#define pokew(a,b,c)	(*((int  far*)MK_FP((a),(b))) = (int)(c))
#define pokeb(a,b,c)	(*((char far*)MK_FP((a),(b))) = (char)(c))
#define peekw(a,b)	(*((int  far*)MK_FP((a),(b))))
#define peekb(a,b)	(*((char far*)MK_FP((a),(b))))

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

typedef enum boolean_tag
{
	FALSE,
	TRUE
}boolean;

#endif
