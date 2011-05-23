#include "interrupt.h"

extern Video video;
extern System system;

Interrupt::Interrupt()
{
	int i = 32;
	int16 selector = 0;
	video.setcolour(system.WinMain, TRUEWHITE | BBLUE);
	video.prints(system.WinMain, "\n  Loading IDT...");

	asm volatile("movw %%cs,%0" :"=g"(selector)); //Obtain Code Selector
	selector_code = selector;

	init_pic();
	set_exceptions();

	for (;i<256;i++) //Fill IDT with default entries.
	{
		set_vector(i,default_handler, INT_GATE|BITS_32|PRESENT|RING_3);
	}

	idt_desc.size=(256*8)-1;
	idt_desc.offset=(dword)idt;

	asm("lidt %0" : "=m" (idt_desc)); //Load IDT.

	current_mask = 0xffff;	//disables all interrupts

	video.prints(system.WinMain, "  [ DONE ]\n");
	video.setcolour(system.WinMain, TCYAN | BBLUE);
}

void Interrupt::init_pic()
{
	outportb(PICM, ICW1);
	outportb(PICS, ICW1);

	outportb(PICMI, MASTER_VEC);
	outportb(PICSI, SLAVE_VEC);

	outportb(PICMI, 4);
	outportb(PICSI, 2);

	outportb(PICMI, ICW4);
	outportb(PICSI, ICW4);

	outportb(PICMI, 0xff);
	outportb(PICSI, 0xff);
}

void Interrupt::set_exceptions()
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
	{
		set_vector(i, cpu15, TRAP_GATE|BITS_32|PRESENT|RING_0);
	}
}

void Interrupt::set_vector(int intno, void (*pointer)(), byte control)
{
	struct addr
	{
		word offset;
		word base;
	};
   union
   {
		void (*function)();
		struct addr function_addr;
	}intrpt;

	intrpt.function = pointer;

	idt[intno].selector = selector_code;
	idt[intno].dword_count = 0;
	idt[intno].type = control;  //  0x0ee;
	idt[intno].offset1 = intrpt.function_addr.offset;
	idt[intno].offset2 = intrpt.function_addr.base;
}

void Interrupt::enable_irq(word x, void (*handler)(), byte control)
{
	current_mask &= ~(1<<x) ; 	// zero off the mask to enable
	if (x>=8)
	{
		current_mask &= ~(1<<2);
		set_vector((x + SLAVE_VEC - 8), handler, control);
	}
	else
	{
		set_vector((x + MASTER_VEC), handler, control);
	}
	outportb(PICMI, current_mask & 0xff);
	outportb(PICSI, (current_mask >> 8) & 0xff);
}

void Interrupt::disable_irq(word x)
{
	current_mask |= (1 << x);
	if(x >= 8)
	{
		current_mask |= (1 << 2);
		set_vector((x + SLAVE_VEC - 8), default_handler, INT_GATE|BITS_32|PRESENT|RING_3);
	}
	else
	{
		set_vector((x + MASTER_VEC), default_handler, INT_GATE|BITS_32|PRESENT|RING_3);
	}
	
	outportb(PICMI, current_mask & 0xFF);
	outportb(PICSI, (current_mask >> 8) & 0xFF);
}


void default_handler()
{
	disable();

	system.assert(system.WinMain, 1, "An unhandled interrupt has occured...");

	enable();

	asm("mov %ebp,%esp");
	asm("pop %ebp");
	asm("iret");
}

void cpu0()
{
	disable();
	system.assert(system.WinMain, 0 , "Divide Error...");
}

void cpu1()
{
	disable();
	system.assert(system.WinMain, 0 , "Debug Exception...");
}

void cpu2()
{
	disable();
	system.assert(system.WinMain, 0 , "Unknown Exception...");
}

void cpu3()
{
	disable();
	system.assert(system.WinMain, 0 , "Unexpected Breakpoint...");
}

void cpu4()
{
	disable();
	system.assert(system.WinMain, 0 , "Overflow Error...");
}

void cpu5()
{
	disable();
	system.assert(system.WinMain, 0 , "Bounds Check Error...");
}

void cpu6()
{
	disable();
	system.assert(system.WinMain, 0 , "Invalid Opcode...");
}

void cpu7()
{
	disable();
	system.assert(system.WinMain, 0 , "Coprocessor Not Available...");
}

void cpu8()
{
	disable();
	system.assert(system.WinMain, 0 , "Double Fault...");
}

void cpu9()
{
	disable();
	system.assert(system.WinMain, 0 , "Coprocessor Segment Overrun...");
}

void cpu10()
{
	disable();
	system.assert(system.WinMain, 0 , "Invalid TSS...");
}

void cpu11()
{
	disable();
	system.assert(system.WinMain, 0 , "Segment Not Present...");
}

void cpu12()
{
	disable();
	system.assert(system.WinMain, 0 , "Stack Exception...");
}

void cpu13()
{
	disable();
	system.assert(system.WinMain, 0 , "General Protection Exception (Triple Fault)...");
}

void cpu14()
{
	disable();
	system.assert(system.WinMain, 0 , "Page Fault...");
}

void cpu15()
{
	disable();
	system.assert(system.WinMain, 0 , "Unknown Exception...");
}

void cpu16()
{
	disable();
	system.assert(system.WinMain, 0 , "Coprocessor Error...");
}
