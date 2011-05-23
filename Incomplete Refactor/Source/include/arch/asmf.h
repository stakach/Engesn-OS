#ifndef __ASMF_H__
#define __ASMF_H__


#include <def.h>

//
// Assembly routines
//


#define enable()	asm volatile("sti"::)
#define disable()	asm volatile("cli"::)
#define halt()		asm volatile("hlt"::)

#define pushAll()	asm volatile("pushal"::)
#define popAll()	asm volatile("popal"::)
#define leave()		asm volatile("leave"::)
#define iret()		asm volatile("iret"::)


inline static UCHAR inportb(INT port)
{
	register UCHAR r;
   
   	asm volatile
	( 
		"inb %%dx, %%al\n\t" 
		: "=a" (r) 
		: "d" (port)
	);

   	return (r);
}

inline static VOID outportb(INT port, UCHAR data)
{
	asm volatile
	(
		"outb %%al, %%dx\n\t" 
		:
		: "a" (data), "d" (port)
	);
}

inline static USHORT inportw(INT port)
{
	register USHORT r;
   
   	asm volatile
	(
		"inw %%dx, %%ax\n\t" 
		: "=a" (r) 
		: "d" (port)
	);

   	return (r);
}

inline static VOID outportw(INT port, USHORT data)
{
	asm volatile
	(
		"outw %%ax, %%dx\n\t" 
		:
		: "a" (data), "d" (port)
	);
}

inline static ULONG inportd(INT port) 
{ 
	register ULONG r;

	asm volatile
	(
		"inl %%dx, %%eax\n\t"
		:"=a" (r)
		:"d" (port)
	); 

	return (r);
}

inline static VOID outportd(INT port, ULONG data) 
{
	asm volatile
	(
		"outl %%eax, %%dx\n\t"
		:
		:"a" (data), "d" (port)
	);
}



//
// Returns current data selector
//

inline static UINT get_ds()
{
	UINT a;
	asm volatile("movw %%ds,%0" :"=g"(a));
	return a;
}


//
// Returns current stack selector
//

inline static UINT get_ss()
{
	UINT a;
	asm volatile("movw %%ss,%0" :"=g"(a));
	return a;
}


//
// Returns current code selector
//

inline static UINT get_cs()
{
	UINT a;
	asm volatile("movw %%cs,%0" :"=g"(a));
	return a;
}


//
// Returns rgisters:
//

inline static ULONG get_esp()
{
	ULONG a;
	asm volatile("mov %%esp,%0" :"=g"(a));
	return a;
}

inline static ULONG get_ebp()
{
	ULONG a;
	asm volatile("mov %%ebp,%0" :"=g"(a));
	return a;
}


//
// System control register access functions:
//

inline static VOID put_cr0(ULONG Cr0)
{
	asm volatile("movl %%eax, %%cr0" :: "a"(Cr0));
}


inline static ULONG get_cr0()
{
	ULONG a;
	asm volatile("mov %%cr0,%%eax" :"=a"(a));
	return a;
}


//
// Paging control.
//

inline static VOID put_cr3(ULONG PageDir)
{
	asm volatile("movl %%eax, %%cr3" :: "a"(PageDir));
}


inline static ULONG get_cr3()
{
	ULONG a;
	asm volatile("mov %%cr3,%%eax" :"=a"(a));
	return a;
}


inline static VOID Invlpg(VOID *m) 
{ 
	asm volatile("invlpg %0"::"m" (m));
} 


inline static VOID put_cr4(ULONG Data)
{
	asm volatile("movl %%eax, %%cr4" :: "a"(Data));
}


inline static ULONG get_cr4()
{
	ULONG a;
	asm volatile("mov %%cr4,%%eax" :"=a"(a));
	return a;
}


//
// Holds last page fault address.
//

inline static ULONG get_cr2()
{
	ULONG a;
	asm volatile("mov %%cr2,%%eax" :"=a"(a));
	return a;
}

#endif
