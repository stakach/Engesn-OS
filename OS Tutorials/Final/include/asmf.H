#ifndef _asmf_H_
#define _asmf_H_

// Assembly routines //
//-------------------//

namespace asmflags
{
	extern bool disabled;
};


inline static unsigned char inportb(int port)
{
	register unsigned char r;
   
   	asm volatile
	( 
		"inb %%dx, %%al\n\t" 
		: "=a" (r) 
		: "d" (port)
	);

   	return (r);
}

inline static void outportb(int port, unsigned char data)
{
	asm volatile
	(
		"outb %%al, %%dx\n\t" 
		:
		: "a" (data), "d" (port)
	);
}

inline static unsigned short inportw(int port)
{
	register unsigned short r;
   
   	asm volatile
	(
		"inw %%dx, %%ax\n\t" 
		: "=a" (r) 
		: "d" (port)
	);

   	return (r);
}

inline static void outportw(int port, unsigned short data)
{
	asm volatile
	(
		"outw %%ax, %%dx\n\t" 
		:
		: "a" (data), "d" (port)
	);
}

inline static unsigned long inportd(int port) 
{ 
	register unsigned long r;

	asm volatile
	(
		"inl %%dx, %%eax\n\t"
		:"=a" (r)
		:"d" (port)
	); 

	return (r);
}

inline static void outportd(int port, unsigned long data) 
{
	asm volatile
	(
		"outl %%eax, %%dx\n\t"
		:
		:"a" (data), "d" (port)
	);
}



inline static void enable()
{
	asmflags::disabled = false;
	asm volatile 
	(
		"sti"
		:
		:
	);
}

inline static void disable()
{
	asmflags::disabled = true;
	asm volatile
	(
		"cli"
		:
		:
	);
}

inline static void halt()
{
	asm volatile
	(
		"hlt"
		:
		:
	);
} // Halts CPU



inline static unsigned int get_ds(void)	//Get data selector
{
	unsigned int a;
	asm volatile("movw %%ds,%0" :"=g"(a));
	return a;
}

inline static unsigned int get_ss(void)	//Get stack selector
{
	unsigned int a;
	asm volatile("movw %%ss,%0" :"=g"(a));
	return a;
}

inline static unsigned int get_cs(void)	//Get code selector
{
	unsigned int a;
	asm volatile("movw %%cs,%0" :"=g"(a));
	return a;
}

inline static unsigned long get_esp(void)
{
	unsigned long a;
	asm volatile("mov %%esp,%0" :"=g"(a));
	return a;
}

inline static unsigned long get_ebp(void)
{
	unsigned long a;
	asm volatile("mov %%ebp,%0" :"=g"(a));
	return a;
}


inline static unsigned char peekb(unsigned long addr)
{
	unsigned char _v;

	asm ("movb %%fs:%1,%0":"=r" (_v):"m" ((char *)addr));
	return _v;
}

inline static unsigned short peekw(unsigned long addr)
{
	unsigned short _v;

	asm ("movw %%fs:%1,%0":"=r" (_v):"m" ((unsigned short *)addr));
	return _v;
}

inline static unsigned long peekl(unsigned long addr)
{
	unsigned long _v;

	asm ("movl %%fs:%1,%0":"=r" (_v):"m" ((unsigned long *)addr)); \
	return _v;
}

inline static void pokeb(unsigned char val,unsigned long addr)
{
	asm ("movb %0,%%fs:%1"::"r" (val),"m" ((unsigned char *)addr));
}

inline static void pokew(unsigned short val,unsigned long addr)
{
	asm ("movw %0,%%fs:%1"::"r" (val),"m" ((unsigned short *)addr));
}

inline static void pokel(unsigned long val,unsigned long addr)
{
	asm ("movl %0,%%fs:%1"::"r" (val),"m" ((unsigned long *)addr));
}


	//inline static void pokeb(unsigned short selector, unsigned long offset, unsigned char value)


#endif
