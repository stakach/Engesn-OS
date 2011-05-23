#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__


//-----------------------
//		PIC Defines:

#define EOI			0x20	//end of interrupt

#define MASTER_VEC	32		//Start of PIC Master interrupt vectors (after cpu reserved interrupts)
#define SLAVE_VEC	40		//Start of PIC Slave interrupt vectors (after pic master interrupts)


//-----------------------
//	IDT control flags:

#define INT_GATE	0x06	//	Every descriptor has one of each of these (eg Gate | Bit | Present | Ring)
#define TRAP_GATE	0x07	//	Type of interrupt gate

#define BITS_16		0x00	//	Type of code within gate
#define BITS_32		0x08

#define ABSENT		0x0		//	Gate present or absent
#define PRESENT		0x80

#define RING_0		0x00	//	Privilege level of code
#define RING_1		0x20
#define RING_2		0x40
#define RING_3		0x60


//-----------------------

struct idt_gate
{
	unsigned short	offset_low;		// offset 0..15
	unsigned short	selector;		// selector
	unsigned char	dword_count;	// 0
	unsigned char	type;			// control flags
	unsigned short	offset_high;	// offset 16..31
} __attribute ((packed));


class Interrupt
{
	public:
		Interrupt();
		void set_vector(int intno, void (*pointer)(), unsigned char control);
		void enable_irq(unsigned short x, void (*handler)(), unsigned char control);
		void disable_irq(unsigned short x);

	private:
		void init_pic();
		void set_exceptions();

		int selector_code;
		long current_mask;
		idt_gate idt[256];

		struct
		{
			unsigned short size	__attribute ((packed));
			idt_gate *offset	__attribute ((packed));
		} idt_desc;
};


extern Interrupt intcontrol;


#endif
