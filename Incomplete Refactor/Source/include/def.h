#ifndef __DEF_H__
#define __DEF_H__


#define KE_STATUS bool
#define STATUS_SUCCESS true
#define STATUS_FAILURE false


#define BOOLEAN bool
#define BOOL bool

#define CHAR char
#define UCHAR unsigned char
#define BYTE unsigned char

#define SHORT short
#define USHORT unsigned short
#define WORD unsigned short

#define LONG long
#define ULONG unsigned long
#define DWORD unsigned long

#define LONGLONG long long
#define ULONGLONG unsigned long long

#define INT int
#define UINT unsigned int

#define VOID void


#define NULL 0


#define CAST(NEW_TYPE, OLD_OBJECT) (*((NEW_TYPE*)(&(OLD_OBJECT))))


#define CHECK_FLAG(Flags, Bit)   ((Flags) & (1 << (Bit)))


#define HIGHWORD(a) ((USHORT)(((ULONG)(a)>>16) & 0xffff))
#define LOWWORD(a) ((USHORT)((ULONG)a & 0x0000ffff))
#define MAKEULONG(a, b) (((((ULONG)(a))<<16) & 0xffff0000) | ((ULONG)(b) & 0xffff))


//
// Binary coded decimal to binary converter.
//

#define Bcd2Dec(BCD) ((((BCD) >> 4) * 0xA) + ((BCD) & 0xf))



/*
//
// Create new sections in the kernel that can be discarded
//	after the kernel has loaded.
//
#define DISCARDABLE_CODE(X)							\
		X __attribute__((section (".dc_start")));	\
													\
		X

#define DISCARDABLE_MEMBER_CODE(X)							\
		X __attribute__((section (".dc_start")))

#define	DISCARDABLE_DATA(X)									\
		extern X __attribute__((section (".dd_start")));	\
															\
		X
*/

#endif
