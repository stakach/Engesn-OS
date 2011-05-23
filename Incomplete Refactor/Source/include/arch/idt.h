#ifndef __IDT_H__
#define __IDT_H__


#include <def.h>
#include <arch/i386.h>


//
// Interrupt controller register addresses.
//
#define PIC1_PA		0x20	// PIC Master Port A
#define PIC2_PA		0xA0	// PIC Slave Port A

#define PIC1_PB		0x21	// PIC Master Port B
#define PIC2_PB		0xA1	// PIC Slave Port B


//
// Commands for Interrupt Controller
//
#define ICW1		0x11	// Control Word 1
#define ICW4		0x01	// Control Word 4 (optional [slave])
#define EOI			0x20	// end of interrupt
#define IN_SERVICE	0x0B	// Request Interrupt number currently being serviced


// Number of Interrupt entries we have
#define MAX_IDT 256


namespace I386
{


	class IDT
	{
		public:
			VOID Load();
			KE_STATUS AddInt(UINT Number, DWORD Handler, DWORD Settings);
			INT AddInt(DWORD Handler, INT AttachToIRQ = -1);
			VOID RemInt(UINT Number);
			UINT NumEntries();
			
			static IDT idt;

		private:
			IDT();
			VOID RemapPic(UINT Pic1, UINT Pic2);

			DT_ENTRY Idt[MAX_IDT];
			DT_POINTER IdtDesc;
			INT NextFreeInterrupt;
	};


}


#endif
