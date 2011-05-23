
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\include\bios32.h"
#include "..\include\OStream.h"	//cout, endl

using std::cout;
using std::endl;


#define BIOS32_SIG	('_'+('3'<<8)+('2'<<16)+('_'<<24))

#define PCI_SIG		('$'+('P'<<8)+('C'<<16)+('I'<<24))
#define PCI_INS		('P'+('C'<<8)+('I'<<16)+(' '<<24))

#define PNP_SIG		('$'+('A'<<8)+('C'<<16)+('F'<<24))
#define WDS_SIG		('$'+('W'<<8)+('D'<<16)+('S'<<24))
#define MPTN_SIG	('M'+('P'<<8)+('T'<<16)+('N'<<24))


PmBios bios32service;

PmBios::PmBios()
{
	union bios32* p;

	avaliable = false;
	dirTable.fields.revision = 0;

	cout << "\n  Detecting BIOS32 service directory...";

	for(p = (union bios32*) 0xE0000; p < (union bios32*) 0x100000; p++)
	{
		if(p->fields.signature == BIOS32_SIG)	//Searches for bios32 header
		{
			unsigned char checksum = 0;
			int i = 0;

			for(; i <= 15; i++)					//Validates header checksum (POP DI; XOR SI,[BP+SI] == BIOS32_SIG)
			{									//We may have just found some code
				checksum += p->chars[i];
			}
			if(checksum == 0)
			{
				dirTable = *p;					//We found the BIOS32 Header
				avaliable = true;
				cout << "\t\t\t[ DONE ]\n";
				cout << "\n\tBIOS32 service" << " (rev " << (unsigned short)dirTable.fields.revision << ") at " << hex << dirTable.fields.entry;
				break;
			}
		}
	}

	if(avaliable)										//If bios32 (revision 0) exists
	{													//determine avaliable services
		if(dirTable.fields.revision == 0)
		{
			if(B32Caller(PCI_SIG, services[0]))
			{
				checkPCI();				//checks PCI service once it is found avaliable
				if(pcibios.installed)
				{
					cout << "\t\tVersion " << dec << pcibios.verhigh << "." << pcibios.verlow << "  Buses " << (pcibios.buses + 1);
				}
			}
			else
			{
				pcibios.installed = false;
			}

			B32Caller(PNP_SIG, services[1]);

			cout << dec << endl;		//Cout assumed dec for next initialiser
		}
		else
		{
			pcibios.installed = false;
			cout << "\n\tUnsupported BIOS32 revision " << dirTable.fields.revision << endl;
		}
	}
	else
	{
		pcibios.installed = false;
		cout << dec << "\t\t\t[ FAIL ]\n";
	}
}

bool PmBios::B32Caller(unsigned long sig, B32_Service &B32service)
{
	unsigned char exist = 0;
	B32service.sig.value = sig;
	B32service.endstr = '\0';

	asm volatile 
		("pushl	%%cs\n"					//puch cs for 'fret'
		"call %%edi"					//call directory entry
		:	"=a" (exist),				//Output:
			"=b" (B32service.base),
			"=c" (B32service.length),
			"=d" (B32service.offset)
		:	"a" (sig),					//Input:
			"b" (0),
			"D" (dirTable.fields.entry)
	);

	if(exist == 0)
	{
		cout << "\n\t\t" << B32service.sig.str << " : " << hex << (B32service.base + B32service.offset);
		B32service.entry.address = B32service.base + B32service.offset;
		return B32service.avaliable = true;
	}
	else
	{
		B32service.entry.address = 0;
		return B32service.avaliable = false;
	}
}

void PmBios::checkPCI()					//Checks pci bios is functional
{
	unsigned long entry, temp, ver;
	pcibios.endstr = '\0';

	asm volatile
		("pushl	%%cs\n"					//puch cs for 'fret'
		"call %%esi"					//call pci bios entry
		:	"=a" (temp),				//Output:
			"=b" (ver),
			"=c" (pcibios.buses),
			"=d" (pcibios.sig.value),
			"=D" (entry)
		:	"a" (0xB101),				//Input:Installation check
			"D" (0),
			"S" (services[0].entry.address)
	);

	if(pcibios.sig.value == PCI_INS)	//If installed fill in structure
	{
		cout << "\t\t\t\t[  OK  ]\n";
		pcibios.installed = true;
		pcibios.verhigh = (ver >> 8) & 255;
		pcibios.verlow = ver & 255;
		pcibios.buses &= 255;
	}
	else
	{
		cout << "\t\t\t\t[ FAIL ]";
		pcibios.installed = false;
	}
}

bool PmBios::present()				//Returns true if bios32 services are present
{
	return avaliable;
}

bool PmBios::pci(PCI_Info &struc)	//Returns true if pci bios services are present
{
	struc = pcibios;
	return pcibios.installed;
}

/*
Call BIOS32 Service Directory entry point with:
	EBX = function
	    00000000h get service entry point
		EAX = service identifier
		    46434124h ("FCA$") Plug-and-Play Auto-Configuration
		    49435024h ("ICP$") PCI BIOS
		    4E54504Dh ("NTPM") ??? MPTN [PhoenixBIOS4 Rev. 6.0]
		    54435724h ("SDW$") ??? WDS$ [PhoenixBIOS4 Rev. 6.0]
		Return: AL = status
			    00h successful
				 EBX = base address of handler's code seg
				 ECX = size of code segment
				 EDX = offset of handler in code seg
			    80h unknown service identifier
	    else
		Return: AL = 81h invalid function
*/
