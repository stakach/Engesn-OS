//Kernel.cpp

#include "include\IOStream.h"	//cout, cin
#include "include\cmos.h"

using std::cout;		//so we only have to write cout.
using std::endl;
using std::cin;

int main(void)
{
	char buffer[256];	//Max input from cin is 256 chars including '\0' char.
	Cmos cmos;
	unsigned char fdrives, a, b;

	cout << "\n\n Compiled using GCC " << __VERSION__ << endl;
	cout << " On " << __DATE__ << endl;

	cout << "\nHello, what is your name? ";

	cin >> buffer;

	cout << "\nHi " << buffer << "! Nice to meet you." << endl;

	fdrives = cmos.read(0x10);
	a = fdrives >> 4;		// get the high nibble
	b = fdrives & 0xF;		// get the low nibble

	cout << "Drive Detection, A=" << (unsigned int)a << ", B=" << (unsigned int)b << endl;
}
/*
void getE820Map()
{
	long maplength;
	e820entry *mem = 0x500;
	unsigned long a;
	unsigned long reservedmemory; 
	unsigned long useablememory; 

	asm volatile("movl %%edi,%0" : "=r" (maplength) );

	for (a = 0; a < (maplength - 0x500) / 20; a++)
	{
		cout << mem[a].addr << '\t' << mem[a].size << '\t' << mem[a].type << endl;
		if (mem[a].type == 1)
		{
			useablememory += mem[a].size;
		}
		else
		{
			reservedmemory += mem[a].size;
		}
	}

	cout << "[RAM] Reserved: " << reservedmemory / 1024 << "KB Useable: " << useablememory / 1024 << "KB Total Memory: " << (reservedmemory+useablememory) / 1024 / 1024 << "MB\n";
	cout << "Max Pages for Physical RAM: [" << (reservedmemory + useablememory) / 4096 + 1 << "]\n";
}
/*
Offset in Bytes		Name		Description
	0	    BaseAddrLow		Low 32 Bits of Base Address
	4	    BaseAddrHigh	High 32 Bits of Base Address
	8	    LengthLow		Low 32 Bits of Length in Bytes
	12	    LengthHigh		High 32 Bits of Length in Bytes
	16	    Type		Address type of  this range.


Value	Pneumonic		Description
1	AddressRangeMemory	This run is available RAM usable by the
				operating system.
2	AddressRangeReserved	This run of addresses is in use or reserved
				by the system, and must not be used by the
				operating system.
Other	Undefined		Undefined - Reserved for future use.  Any
				range of this type must be treated by the
				OS as if the type returned was
				AddressRangeReserved.

*/
