/*++

Copyright (c) 2003-2004  Engesn OS Derivative

Module Name:

	mboot.cpp

Abstract:

	This module contains all multiboot relevent routines and data structures
	for use with kernel initialisation.

Author:

	Stephen von Takach 14-July-2004

Revision History:


--*/


#include <def.h>
#include <hal.h>
#include <arch/mboot.h>
#include <arch/i386.h>
#include <arch/pager.h>


#define MB_MAGIC 0x2BADB002


	#ifdef __cplusplus
		extern "C" 
		{
	#endif 

			//
			// Information recieved from boot loader if multiboot
			//	NOTE :: Variables that don't conform to coding
			//			standards are external (assembly).
			//

			extern ULONG mboot;
			extern ULONG mbinfo;
			extern ULONG mbmagic;

	#ifdef __cplusplus
		}
	#endif


//
// Global pointers hold boot information 
//

const MULTIBOOT_HEADER	*MbootHeader = (MULTIBOOT_HEADER *) &mboot;
const MULTIBOOT_INFO	*MbootInfo = (MULTIBOOT_INFO *) mbinfo;


namespace I386
{

	KE_STATUS CheckMboot()
	{

		BOOL MemSetup = false;


		//
		// Checks if the OS was loaded by a multiboot compliant loader
		//	then displays amount of memory and checks if any modules were
		//	loaded.
		//

		if(mbmagic != MB_MAGIC)
		{
			I386HAL::SystemHal.CriticalException("Multiboot loader check error", 33, 1);
			return (STATUS_FAILURE);
		}

		if (CHECK_FLAG(MbootInfo->Flags, 0))
		{
			HAL::Console->Write("Base memory: ");
			HAL::Console->Write(MbootInfo->MemLower, 10);
			HAL::Console->Write("kb\tExtended memory: ");
			HAL::Console->Write(MbootInfo->MemUpper, 10);
			HAL::Console->Write("kb\n");
			
			//
			// Check for a memory map
			//
			if(CHECK_FLAG (MbootInfo->Flags, 6))
			{
				MEMORY_MAP *Mmap;
			  
				//
				// Map the memory map into our physical memory allocator
				//
				for (Mmap = (MEMORY_MAP *)MbootInfo->MmapAddr; (ULONG)Mmap < MbootInfo->MmapAddr + MbootInfo->MmapLength; Mmap = (MEMORY_MAP *)((ULONG) Mmap + Mmap->Size + sizeof(Mmap->Size)))
				{
					PAGER::KernelMM.RealMem.MapMemory(Mmap->BaseAddr, Mmap->Length, Mmap->Type);
				}
			}
			else
			{
				//
				// If there is no memory map, we must guess :(
				//
				PAGER::KernelMM.RealMem.MapMemory((ULONGLONG)0x100000, (ULONGLONG)MbootInfo->MemUpper, 1);
			}

			//
			// Inform the memory allocator we are finished.
			//
			PAGER::KernelMM.SetMemory();

			MemSetup = true;
		}

		if(!CHECK_FLAG(MbootInfo->Flags, 3))
		{
			// Map Drivers into memory here.
			I386HAL::SystemHal.CriticalException("Multiboot driver load error", 34, 1);
		}

		return MemSetup;
	}


}
