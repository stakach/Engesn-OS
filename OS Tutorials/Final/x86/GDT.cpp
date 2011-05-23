
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\include\GDT.h"
#include "..\include\OStream.h"	//cout

#define GDT_EMPTY 0		//empty
#define GDT_FULL 1		//full

using std::cout;

GDT gdt;				//Global control for adding removing descriptors.

unsigned int GDT::findEmpty()	// Returns zero if there are no free descriptors
{
	unsigned int i;
	for (i = 0; i < MAX_ELEM; i++)
	{
		if (gdt[i] == GDT_EMPTY) 
		{
			gdt[i] = GDT_FULL;
			return i;
		}
	}
	return 0;
}

unsigned int GDT::addDesc(unsigned int base, unsigned int limit, unsigned int opt)
{
	unsigned int i = findEmpty();	//finds first free descriptor and validates it.
	GDTable[i].desc.base_low = base & 0xFFFF;
	GDTable[i].desc.base_med = (base >> 16) & 0xFF;
	GDTable[i].desc.base_high = base >> 24;
	GDTable[i].desc.limit_low = limit & 0xFFFF;
	GDTable[i].desc.limit_high = (limit >> 16) & 0x0F;
	GDTable[i].desc.access = (opt + D_PRESENT) >> 8;
	GDTable[i].desc.granularity = ((opt & 0xff) >> 4);
	return i * 8;
}

unsigned int GDT::remDesc(unsigned int selector)
{
	selector /= 8;
	gdt[selector] = GDT_EMPTY;		//Marks descriptor as free
	GDTable[selector].dummy = 0;	//Sets descriptor to null
}

GDT::GDT()
{
	unsigned int i;

	cout << "\n  Loading GDT...";

	gdt_desc.size = (MAX_ELEM * sizeof(union DT_entry) - 1);
	gdt_desc.offset = GDTable;	//Fills our GDT Descriptor

	for (i = 0; i < MAX_ELEM; i++)
	{
		gdt[i] = GDT_EMPTY;		//Mark remaining entries as free
	}

	gdt[0] = GDT_FULL;			//The first element of the gdt is not usable and is made null
	GDTable[0].dummy = 0;		//Easy done.


	addDesc(0, 0xFFFFFFFF, (D_CODE + D_READ + D_BIG + D_BIG_LIM));		//Kernel code descriptor
	addDesc(0, 0xFFFFFFFF, (D_DATA + D_WRITE + D_BIG + D_BIG_LIM));		//Kernel data\stack descriptor


	asm volatile ("lgdt %0" : "=m" (gdt_desc));	//Loads our GDT

	cout << "\t\t\t\t\t[ DONE ]\n";
}

