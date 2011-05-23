
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "..\include\ACPI.h"
#include "..\include\OStream.h"	//cout, endl

using std::cout;
using std::endl;

ACPIFunc acpiService;

ACPIFunc::ACPIFunc()							//Advanced Configuration and Power Interface detection
{
	union ACPI* p;
	unsigned char *address = (unsigned char *)0xE0000;
	
	found = false;

	cout << "\n  Detecting ACPI System Description Pointer...";

	while(!found && ((unsigned long)address < 0x100000))
	{
		p = (union ACPI*)address;
		//p = (union ACPI*)0x40E;
							//"RSD "							"PTR "
		if((p->fields.sig1 == 0x20445352) && (p->fields.sig2 == 0x20525450))	//Searches for ACPI header
		{
			unsigned char checksum = 0;
			int i = 0;
			int ii = 19;

			if(p->fields.revision == 0)
			{
				cout << "found";
			}
			else if(p->fields.revision == 2)
			{
				ii = 35;
				cout << "found";
			}

			for(; i <= ii; i++)					//Validates header checksum
			{									//We may have just found some code
				checksum += p->chars[i];
			}

			if(checksum == 0)
			{
				desTable = *p;					//We found the ACPI Header
				found = true;
				cout << "\t\t[ DONE ]\n";
				cout << "\n\tACPI pointer" << " (rev " << (unsigned short)desTable.fields.revision << ") at " << hex << (unsigned long)address << dec << endl;
				break;
			}
		}
		address += 0x10;
	}

	if(!found)
	{
		cout << "\t\t[ FAIL ]\n";
	}
}

