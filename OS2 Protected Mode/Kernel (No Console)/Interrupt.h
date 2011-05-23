#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "kerndefs.h"
#include "asmf.h"
#include "Pkernel.h"
#include "Video.h"


#define EOI			0x20	//end of interrupt


class Interrupt
{
	public:
		Interrupt();
		void set_vector(int intno, void (*pointer)(), byte control);
		void enable_irq(word x, void (*handler)(), byte control);
		void disable_irq(word x);

	private:
		void init_pic();
		void set_exceptions();

		int16 selector_code;
		int32 current_mask;

		struct idt_gate
		{
			word offset1;
			word selector;
			byte dword_count;
			byte type;
			word offset2;
		} __attribute ((packed));
		idt_gate idt[256];

		struct
		{
			word size		__attribute ((packed));
			dword offset	__attribute ((packed));
		} idt_desc;

		#define MASTER_VEC	32
		#define SLAVE_VEC	40

		#define PICM		0x20
		#define PICMI		0x21
		#define PICS		0xA0
		#define PICSI		0xA1

		#define ICW1		0x11
		#define ICW4		0x01

		//-----------------------

		#define INT_GATE 0x06     
		#define TRAP_GATE 0x07

		#define BITS_16 0x00
		#define BITS_32 0x08

		#define ABSENT 0x0
		#define PRESENT 0x80

		#define RING_0 0x00
		#define RING_1 0x20
		#define RING_2 0x40
		#define RING_3 0x60
};



void default_handler();
void cpu0();
void cpu1();
void cpu2();
void cpu3();
void cpu4();
void cpu5();
void cpu6();
void cpu7();
void cpu8();
void cpu9();
void cpu10();
void cpu11();
void cpu12();
void cpu13();
void cpu14();
void cpu15();
void cpu16();


#endif
