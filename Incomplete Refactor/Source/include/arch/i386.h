#ifndef __I386_H__
#define __I386_H__


#include <def.h>
#include <hal.h>



//
// Descriptor privilege levels and presences
//

#define RING_0		0x0000	//	Privilege level of code
#define RING_1		0x2000
#define RING_2		0x4000
#define RING_3		0x6000

#define ABSENT		0x0000	//	Gate present or absent
#define PRESENT		0x8000


//
//	Descriptor access privileges constants
//

#define D_ACC      0x100	// Accessed (Data or Code)

#define D_WRITE    0x200	// Writable (Data segments only)
#define D_READ     0x200	// Readable (Code segments only)
#define D_BUSY     0x200	// Busy (TSS only)

#define D_EXDOWN   0x400	// Expand down (Data segments only)
#define D_CONFORM  0x400	// Conforming (Code segments only)

#define D_BIG      0x40		// Default to 32 bit mode
#define D_BIG_LIM  0x80		// Limit is in 4K units


//
// Gate Types (IDT):
//

#define TASK_G  0x0500	// Task gate
#define CALL_G  0x0C00	// 386 call gate
#define INT_G   0x0E00	// 386 interrupt gate
#define TRAP_G  0x0F00	// 386 trap gate


//
// NonGate Types (GDT):
//

#define D_LDT   0x0200	// LDT segment
#define D_TSS   0x0900	// TSS

#define D_DATA  0x1000	// Data segment
#define D_CODE  0x1800	// Code segment


// 
// 386 hardware structures
//

struct X86_GATE
{
	WORD	LowOffset;		// offset 0..15
	WORD	Selector;		// selector
	WORD	Access;			// access flags
	WORD	HighOffset;		// offset 16..31
} __attribute__ ((packed));


struct X86_DESC
{
	WORD		LimitLow;		// limit 0..15
	WORD		BaseLow;		// base  0..15
	BYTE		BaseMed;		// base  16..23
	BYTE		Access;			// access byte
	ULONG	LimitHigh:4;	// limit 16..19
	ULONG	Granularity:4;	// granularity
	BYTE		BaseHigh;		// base 24..31
} __attribute__ ((packed));


union DT_ENTRY
{
	struct	X86_DESC Desc;		// Normal descriptor
	struct	X86_GATE Gate;		// Gate descriptor
	unsigned long long Dummy;
} __attribute__ ((packed));

struct DT_POINTER
{
	WORD Limit;
	DT_ENTRY *Base;
} __attribute__ ((packed));		// Pointer to Idt or Gdt & limit for the CPU.


struct X86_SELECTOR
{
	union
	{
		struct
		{
			WORD Value;				// Selector value.
		};
		struct
		{
			ULONG Rpl		:  2;	// Requested privilege level.
			ULONG Ti		:  1;	// Table indicator: 0 = GDT, 1 = LDT.
			ULONG Index		: 13;	// Index into descriptor table.
		};
	};
} __attribute__ ((packed));


struct TSS
{
	DWORD	link,
			esp0,
			ss0,
			esp1,
			ss1,
			esp2,
			ss2,
			cr3,
			eip,
			eflags,
			eax,
			ecx,
			edx,
			ebx,
			esp,
			ebp,
			esi,
			edi,
			es,
			cs,
			ss,
			ds,
			fs,
			gs,
			ldtr;
	WORD	trace,
			io_map_addr;
} __attribute__ ((packed));


#include <arch/gdt.h>


class I386HAL: public HAL
{
	public:
		virtual KE_STATUS Setup();
		virtual VOID Shutdown(SHUTDOWN_TYPE Type);
		virtual MEMORY *GetAddressSpace(BOOL UserMode);
		static VOID CriticalException(CHAR *ErrorMsg, INT ErrorCode, INT ErrorType);

		static I386HAL SystemHal;
	private:
		I386HAL();
};


namespace I386
{


	extern VOID Terminate(INT Type);


}


#endif
