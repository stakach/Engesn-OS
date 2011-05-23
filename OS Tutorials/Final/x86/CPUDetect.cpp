
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\include\CPUDetect.h"
#include "..\include\OStream.h"	//cout, cin

using std::cout;				//so we only have to write cout.
using std::endl;


struct REGS
{
	unsigned long eax;
	unsigned long ebx;
	unsigned long ecx;
	unsigned long edx;
};


namespace
{
	void cpuid(unsigned long level, REGS &regs)
	{
		asm volatile
		(	"cpuid"					//This instruction will load our registers with the data we need
			:	"=a" (regs.eax),	//Output:
				"=b" (regs.ebx),
				"=c" (regs.ecx),
				"=d" (regs.edx)
			:	"a" (level)			//Input:
		);
	}

	int cpu486_available()				//Checks 486 cpu is avaliable
	{
		int rc = 0;

		asm volatile
		(	"pushfl\n"
			"popl %%eax\n"
			"movl %%eax, %%ecx\n"
			"xorl $0x40000, %%eax\n"
			"pushl %%eax\n"
			"popfl\n"
			"pushfl\n"
			"popl %%eax\n"
			"subl %%ecx, %%eax\n"
			"jz 1f\n"
			"movl $1, %%eax\n"
			"jmp 2f\n"
			"1:\n"
			"xorl %%eax, %%eax\n"
			"2:\n"
			"xorl $1, %%eax\n"
			"pushl %%ecx\n"
			"popfl\n"
			:	"=a" (rc)
			::	"ecx", "memory"
		);

		return rc;					//Returns 0 if 486 is avaliable
	}

	int cpuid_available()			//Checks cpuid instruction is avaliable (for older cpu's)
	{
		int rc = cpu486_available();

		if(!(rc == 0))
		{
			return rc;
		}

		asm volatile 
		(	"pushfl\n"
			"popl	%%eax\n"
			"movl	%%eax, %%ecx\n"
			"xorl	$0x200000, %%eax\n"
			"pushl	%%eax\n"
			"popfl\n"
			"pushfl\n"
			"popl	%%eax\n"
			"subl	%%ecx, %%eax\n"
			"jz	1f\n"
			"movl	$1, %%eax\n"
			"jmp	2f\n"
			"1:\n"
			"xorl	%%eax, %%eax\n"
			"2:\n"
			"xorl	$1, %%eax\n"
			"pushl	%%ecx\n"
			"popfl\n"
			:	"=a" (rc)
			::	"ecx", "memory"
		);

		return rc;						//Returns 0 if instruction is avaliable
	}
}


CPU cpuInfo;


CPU::CPU()
{
	REGS IDregs;
	bool id80 = false;

	cout << "\n  Requesting CPU information...";

	if(cpuid_available() == 0)			//Skip this if instruction not avaliable (else causes invalid opcode)
	{
		cpuid(0, IDregs);				//returns vendor id and max eax

		MaxEAX = IDregs.eax;
		cpuInfo.vendor.regs.ebx = IDregs.ebx;
		cpuInfo.vendor.regs.ecx = IDregs.ecx;
		cpuInfo.vendor.regs.edx = IDregs.edx;
		cpuInfo.vendor.sign[12] = '\0';

		if(MaxEAX > 0)
		{
			cpuid(1, IDregs);			//returns cpu specific information

			cpuInfo.family = (IDregs.eax & 0xF00) >> 8;
			cpuInfo.familyEx = ((IDregs.eax >> 20) & 0xFF);
			cpuInfo.model = (IDregs.eax & 0xF0) >> 4;
			cpuInfo.modelEx = ((IDregs.eax >> 16) & 0xF);
			cpuInfo.stepping = (IDregs.eax & 0xF);
			cpuInfo.brand = (IDregs.ebx & 0xFF);
			cpuInfo.flags = IDregs.edx;
		}

		cpuid(0x80000000, IDregs);		//checks for extended cpu id

		if(IDregs.eax >= 0x80000004)	//uses extended functions to return cpu name
		{
			cpuid(0x80000002, IDregs);
			*(unsigned long *)(cpuInfo.name + 0 ) = IDregs.eax;
			*(unsigned long *)(cpuInfo.name + 4 ) = IDregs.ebx;
			*(unsigned long *)(cpuInfo.name + 8 ) = IDregs.ecx;
			*(unsigned long *)(cpuInfo.name + 12) = IDregs.edx;

			cpuid(0x80000003, IDregs);
			*(unsigned long *)(cpuInfo.name + 16) = IDregs.eax;
			*(unsigned long *)(cpuInfo.name + 20) = IDregs.ebx;
			*(unsigned long *)(cpuInfo.name + 24) = IDregs.ecx;
			*(unsigned long *)(cpuInfo.name + 28) = IDregs.edx;

			cpuid(0x80000004, IDregs);
			*(unsigned long *)(cpuInfo.name + 32) = IDregs.eax;
			*(unsigned long *)(cpuInfo.name + 36) = IDregs.ebx;
			*(unsigned long *)(cpuInfo.name + 40) = IDregs.ecx;
			*(unsigned long *)(cpuInfo.name + 44) = IDregs.edx;

			cpuInfo.name[64] = '\0';
			id80 = true;
		}

		cout << "\t\t\t\t[ DONE ]\n";	//Prints basic cpu information
		cout << '\t' << cpuInfo.vendor.sign << endl;
		if(MaxEAX > 0)
		{
			cout << "\tFamily: " << (unsigned long)(80086 + (cpuInfo.family * 100)) << "  Model: " << (unsigned short)cpuInfo.model << endl;

			if(id80)
			{
				cout << '\t' << cpuInfo.name << endl;
			}
		}
	}
	else
	{
		cout << "\t\t\t\t[ FAIL ]\n";
	}
}


CPUdata CPU::GetCPU()					// returns cpu information structure
{
	return cpuInfo;
}















