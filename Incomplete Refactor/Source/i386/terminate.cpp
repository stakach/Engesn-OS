/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	terminate.cpp

Abstract:

	This module contains code required to shutdown or re-start the hradware.

Author:

	Stephen von Takach 28-April-2004

Revision History:

--*/


#include <def.h>
#include <hal.h>
#include <arch/asmf.h>
#include <debug/video.h>


	#ifdef __cplusplus
		extern "C" 
		{
	#endif 

			void _atexit();

	#ifdef __cplusplus
		}
	#endif


namespace
{


	UCHAR BiosReset[] =
	{

		//
		// Jumps to real mode and pulses reset line.
		//

		0x66, 0x0f, 0x20, 0xc0,						// movl %cr0,%eax
		0x66, 0x83, 0xe0, 0x11,						// andl $0x00000011,%eax
		0x66, 0x0d, 0x00, 0x00, 0x00, 0x60,			// orl $0x60000000,%eax
		0x66, 0x0f, 0x22, 0xc0,						// movl %eax,%cr0
		0x66, 0x0f, 0x22, 0xd8,						// movl %eax,%cr3
		0x66, 0x0f, 0x20, 0xc3,						// movl %cr0,%ebx
		0x66, 0x81, 0xe3, 0x00, 0x00, 0x00, 0x60,	// andl $0x60000000,%ebx
		0x74, 0x02,									// jz f
		0x0f, 0x08,									// invd
		0x24, 0x10,									// f: andb $0x10,al
		0x66, 0x0f, 0x22, 0xc0,						// movl %eax,%cr0
		0xea, 0x00, 0x00, 0xff, 0xff				// ljmp $0xffff,$0x0000
	};


}


namespace I386
{


	VOID Terminate(INT Type)
	{

		//
		// Hardware restarts or halts the computer.
		// Values for 'Type' == 0 shutdown, 1 Warm boot, (Other) Cold boot.
		//

		USHORT *BiosMem = (USHORT *)0x472;
		void (* JumpBoot)();


		//
		// BiosReset contains the binary code for pulsing the CPU reset line.
		//

		JumpBoot = (void (*)())BiosReset;

		HAL::Console->Write("\nSystem Terminated.\n");
		DEBUG::VIDOUT::Vidout.CursorVisible(false);


		//
		// Call kernel global de-constructors then makes sure interrupts are disabled.
		//

		_atexit();
		disable();
	

		//
		// Set the shutdown type in bios memory then pulse the reset-line or hang.
		//

		if(Type)
		{
			switch (Type)
			{
			case 1:
					*BiosMem = 0x1234;	// Warm boot
					break;
			default:
					*BiosMem = 0x0000;	// Cold boot
			}

			JumpBoot();
		}

		while(1)
		{
			halt();						// Halt the cpu
		}
	}


}

/*
+Values for BiosMem+
--------------------
	0x0000 - Cold boot } ____| Only these options
	0x1234 - Warm boot }     | are used.
	0x4321 - PS/2
	0x5678 - Suspend system
	0x9abc - Manufacturing test mode
	0xabcd - POST loop mode
	0x0064 - Burn-in mode
*/
