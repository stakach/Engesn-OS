
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\include\interrupt.h"
#include "..\include\asmf.h"		//disable, enable, outportb
#include "..\include\end.h"			//kshutdown
#include "..\include\OStream.h"		//cout
#include "..\include\semaphore.h"	//semStart, semEnd

using std::cout;


//-----------------------
//		PIC Defines:
#define PICM		0x20	//PIC Master Port A
#define PICMI		0x21	//PIC Master Port B
#define PICS		0xA0	//PIC Slave Port A
#define PICSI		0xA1	//PIC Slave Port B

#define ICW1		0x11	//Control Word 1
#define ICW4		0x01	//Control Word 4 (optional [slave])


namespace //No-name namespace instead of static keyword
{
	void default_handler();
	void cpu0();
	void cpu1();
	void cpu2();		//CPU interrupt functions
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


	void default_handler()	//Handles any interrupts that have not been set.
	{						//This function should never run...
		disable();

		cout << "\nAn unhandled interrupt has occured...\n";

		enable();
	/*
		asm volatile ("mov %ebp,%esp");
		asm volatile ("pop %ebp");	//Redundant due to leave
	*/
		asm volatile ("leave");
		asm volatile ("iret");
	}

	void cpu0()
	{
		disable();
		cout << "\nDivide Error...\n";
		kshutdown(0);
	}

	void cpu1()
	{
		disable();
		cout << "\nDebug Exception...\n";
		kshutdown(0);
	}

	void cpu2()
	{
		disable();
		cout << "\nUnknown Exception...\n";
		kshutdown(0);
	}

	void cpu3()
	{
		disable();
		cout << "\nUnexpected Breakpoint...\n";
		kshutdown(0);
	}

	void cpu4()
	{
		disable();
		cout << "\nOverflow Error...\n";
		kshutdown(0);
	}

	void cpu5()
	{
		disable();
		cout << "\nBounds Check Error...\n";
		kshutdown(0);
	}

	void cpu6()
	{
		disable();
		cout << "\nInvalid Opcode...\n";
		kshutdown(0);
	}

	void cpu7()
	{
		disable();
		cout << "\nCoprocessor Not Available...\n";
		kshutdown(0);
	}

	void cpu8()
	{
		disable();
		cout << "\nDouble Fault...\n";
		kshutdown(0);
	}

	void cpu9()
	{
		disable();
		cout << "\nCoprocessor Segment Overrun...\n";
		kshutdown(0);
	}

	void cpu10()
	{
		disable();
		cout << "\nInvalid TSS...\n";
		kshutdown(0);
	}

	void cpu11()
	{
		disable();
		cout << "\nSegment Not Present...\n";
		kshutdown(0);
	}

	void cpu12()
	{
		disable();
		cout << "\nStack Exception...\n";
		kshutdown(0);
	}

	void cpu13()
	{
		disable();
		cout << "\nGeneral Protection Exception (Triple Fault)...\n";
		kshutdown(0);
	}

	void cpu14()
	{
		disable();
		cout << "\nPage Fault...\n";
		kshutdown(0);
	}

	void cpu15()
	{
		disable();
		cout << "\nUnknown Exception...\n";
		kshutdown(0);
	}

	void cpu16()
	{
		disable();
		cout << "\nCoprocessor Error...\n";
		kshutdown(0);
	}
}


Interrupt intcontrol;	//Global control for setting interrupts and irq's.

Interrupt::Interrupt()
{
	int i = 32;

	cout << "\n  Loading IDT...";

	selector_code = get_cs();

	init_pic();
	set_exceptions();

	for (;i<256;i++)		//Fill IDT with default entries.
	{
		set_vector(i,default_handler, INT_GATE|BITS_32|PRESENT|RING_3);
	}

	idt_desc.size = (256 * sizeof(idt_gate) - 1);
	idt_desc.offset = idt;

	asm volatile ("lidt %0" : "=m" (idt_desc));	//Load IDT

	current_mask = 0xFFFF;	//disables all interrupts

	enable();

	cout << "\t\t\t\t\t[ DONE ]\n";
}

void Interrupt::init_pic()	//re-programs the pic
{							//IRQ interrupts start at MASTER_VEC
	outportb(PICM, ICW1);
	outportb(PICS, ICW1);

	outportb(PICMI, MASTER_VEC);
	outportb(PICSI, SLAVE_VEC);

	outportb(PICMI, 4);
	outportb(PICSI, 2);

	outportb(PICMI, 0x05);
	outportb(PICSI, ICW4);

	outportb(PICMI, 0xFB);	//line 2 (PIC Slave) enabled
	outportb(PICSI, 0xFF);
}

void Interrupt::set_exceptions()	//Sets CPU interrupts in the IDT
{
	int i = 17;
	set_vector(0, cpu0, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(1, cpu1, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(2, cpu2, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(3, cpu3, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(4, cpu4, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(5, cpu5, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(6, cpu6, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(7, cpu7, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(8, cpu8, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(9, cpu9, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(10, cpu10, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(11, cpu11, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(12, cpu12, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(13, cpu13, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(14, cpu14, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(15, cpu15, TRAP_GATE|BITS_32|PRESENT|RING_0);
	set_vector(16, cpu16, TRAP_GATE|BITS_32|PRESENT|RING_0);
	for(; i < 31; i++)
	{				//Sets the reserved CPU interrupts
		set_vector(i, cpu15, TRAP_GATE|BITS_32|PRESENT|RING_0);
	}
}

void Interrupt::set_vector(int intno, void (*pointer)(), unsigned char control)
{
	semStart();

	struct addr			//Adds interrupt information to the IDT
	{
		unsigned short base;
		unsigned short offset;
	};
   union
   {
		void (*function)();
		struct addr function_addr;
	}intrpt;

	intrpt.function = pointer;

	idt[intno].selector = selector_code;
	idt[intno].dword_count = 0;
	idt[intno].type = control;
	idt[intno].offset_low = intrpt.function_addr.base;
	idt[intno].offset_high = intrpt.function_addr.offset;
	
	semEnd();
}

void Interrupt::enable_irq(unsigned short x, void (*handler)(), unsigned char control)
{
	semStart();

	current_mask &= ~(1 << x);	//Zero off the IRQ mask to enable
	if (x >= 8)
	{
		current_mask &= ~(1 << 2);
		set_vector((x + SLAVE_VEC - 8), handler, control);
	}
	else						//Sets the appropriate interrupt at the same time
	{
		set_vector((x + MASTER_VEC), handler, control);
	}
	outportb(PICMI, current_mask & 0xFB);
	outportb(PICSI, (current_mask >> 8) & 0xFF);

	semEnd();
}

void Interrupt::disable_irq(unsigned short x)
{								//Re-masks an interrupt
	semStart();

	current_mask |= (1 << x);
	if(x >= 8)
	{
		current_mask |= (1 << 2);
		set_vector((x + SLAVE_VEC - 8), default_handler, INT_GATE|BITS_32|PRESENT|RING_3);
	}
	else						//Removes prevous handler
	{
		set_vector((x + MASTER_VEC), default_handler, INT_GATE|BITS_32|PRESENT|RING_3);
	}
	
	outportb(PICMI, current_mask & 0xFB);
	outportb(PICSI, (current_mask >> 8) & 0xFF);

	semEnd();
}

