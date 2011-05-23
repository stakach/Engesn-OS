/*++

Copyright (c) 2003-2006  Engesn OS Derivative

Module Name:

	pager.cpp

Abstract:

	Kernel memory management routines

Author:

	Stephen von Takach 14-Sep-2006

Revision History:

--*/


#include <def.h>
#include <hal.h>
#include <arch/pager.h>
#include <arch/asmf.h>
#include <arch/I386.h>


using namespace I386;


	#ifdef __cplusplus
		extern "C" 
		{
	#endif 


		extern PAGE_ENTRY end;


	#ifdef __cplusplus
		}
	#endif


PAGER PAGER::KernelMM;
PAGER::PHYSMEM PAGER::RealMem;


PAGER::PAGER()
{
	PageSize = 0x1000;			// Tell our base class the page size
	put_cr4(get_cr4() | 0x10 | 0x20 | 0x80);	// Enable Page Size Extensions (2mb Pages), Paging extentions (0x20 bit 5) and Global pages (0x80 bit 7)
}


VOID PAGER::SetMemory()
{
	RealMem.SetMemory();	// Inform RealMem we have finished mapping the memory

	//
	// Setup virtual memory for the kernel
	//
	UINT PDirPointer, DirAddress, PaeAddress;
	if(RealMem.GetPage(PDirPointer) && RealMem.GetPage(DirAddress) && RealMem.GetPage(PaeAddress))
	{
		DirPointer = (PAGE_ENTRY_AE *)PDirPointer;
		
		//
		// General constants for kernel mode paging
		//
		CurDir = (PAGE_ENTRY_AE *)0x3FE00000;
		PageTablesTable = (PAGE_ENTRY_AE *)0x3FFFF000;
		AddressOfPages = 0x3FE00000;
		baseAddress = 0;
		numDirEntries = 511;	//Subtracted the one that holds the page tables

		//
		// Clear page directory pointer (first 1 of 4 is kernel)
		//
		int i = 0;
		for(; i < 512; i++)
		{
			DirPointer[i].Dummy = 0;
		}

		//
		// Setup kernel page directory pointer
		//
		DirPointer[0].Page.Present = 1;
		DirPointer[0].Page.Address = DirAddress >> 12;


		//
		// Clear page directory (entire directory is kernel)
		//
		CurDir = (PAGE_ENTRY_AE *)DirAddress;
		for(i = 0; i < 512; i++)
		{
			CurDir[i].Dummy = 0;
		}

		//
		// Setup page directory with 2mb pages,
		//	mapping the current kernel into memory
		//
		i = 0;
		int j = 0;
		int k = 8;	//16 = 2mb Start at 1mb mark
		for(; i < (ULONG)&end; j++, i += 0x200000)
		{
			CurDir[j].Page.Present = 1;
			CurDir[j].Page.Write = 1;
			CurDir[j].Page.PageSize = 1;
			CurDir[j].Page.Address = i >> 12;

			//
			// Mark the memory used by the kernel
			//	There is some excess, this becomes the kernel Heap
			//
			for(; k < ((j + 1) * 16); k++)
			{
				RealMem.MemUsage[k] = 0xFFFFFFFF;
			}
		}

		//
		// Tell the kernel where the next free entries are
		//
		if(CurDir[j].Page.Present == 0)
			NextDirIndex = j;
		else
			NextDirIndex = j + 1;
		NextPaeIndex = 0;
		NextDirPointer = 0;

		//
		// Add the excess to the heap
		//
		HAL::KernelMm->AddToHeap((UINT)&end, (UINT)(i - 0x200000 - (UINT)&end));

		//
		// Add the page table to kernel virtual space
		//
		CurDir[511].Page.Present = 1;
		CurDir[511].Page.Write = 1;
		CurDir[511].Page.Address = PaeAddress >> 12;

		//
		// Empty the page table
		//
		CurPae = (PAGE_ENTRY_AE *)PaeAddress;
		for(i = 0; i < 512; i++)
		{
			CurPae[i].Dummy = 0;
		}

		//
		// Add the page directory and page table to the
		//	Virtual Address space (Kind of crazy)
		//
		CurPae[0].Page.Present = 1;
		CurPae[0].Page.Write = 1;
		CurPae[0].Page.Address = DirAddress >> 12;

		//
		// Map the page to the allocated address space
		//
		CurPae[511].Page.Present = 1;
		CurPae[511].Page.Write = 1;
		CurPae[511].Page.Address = PaeAddress >> 12;


		//
		// Map the directory pointer to virtual memory
		//
		if(NextDirIndex > 1)
		{
			CurPae[1].Page.Present = 1;
			CurPae[1].Page.Write = 1;
			CurPae[1].Page.Address = PDirPointer >> 12;

			//
			// Get the virtual (Accessable to us, address of the directory)
			//
			VDirPointer = (PAGE_ENTRY_AE *)0x3FE00000 + 0x1000; //Address of kernel directory 3FE00000
		}
		else
		{
			UINT Pae2Address;
			if(RealMem.GetPage(Pae2Address))
			{
				//
				// Enter the new page drectory into the table
				//
				CurPae[1].Page.Present = 1;
				CurPae[1].Page.Write = 1;
				CurPae[1].Page.Address = Pae2Address >> 12;

				//
				// Empty the page table
				//
				CurPae = (PAGE_ENTRY_AE *)Pae2Address;
				for(i = 0; i < 512; i++)
				{
					CurPae[i].Dummy = 0;
				}

				//
				// Create a virtual entry for the directory pointer
				//
				CurPae[0].Page.Present = 1;
				CurPae[0].Page.Write = 1;
				CurPae[0].Page.Address = PDirPointer >> 12;

				NextPaeIndex++;

				//
				// Get the virtual (Accessable to us, address of the directory)
				//
				VDirPointer = (PAGE_ENTRY_AE *)0x200000;
			}
			else
				goto PagerError;
		}


		//
		// Put page directory into cr3, then enable paging in cr0.
		//
		put_cr3((ULONG)PDirPointer);
		put_cr0(get_cr0() | 0x80000000);	// Enable Paging.
	}
	else
		goto PagerError;

	return;

PagerError:
	I386HAL::SystemHal.CriticalException("Paging initialisation error", 36, 0);
}


PAGER::PAGER(BOOL UserMode)
{
	//
	// Get pages needed to setup a basic program address space
	//
	UINT Address = (UINT)KernelMM.GetPages(8, true);
	if(Address == 0)
		goto TaskPageError;

	//
	// Set the variables
	//
	NextDirIndex = 0;
	NextDirPointer = 1;
	NextPaeIndex = 0;
	CurDir = VDirPointer + 0x1000;
	PageTablesTable = (PAGE_ENTRY_AE *)0xFFFFD000;
	AddressOfPages = 0xFFA00000;
	baseAddress = 0x40000000;
	numDirEntries = 1533;	//We subtracted the 3 that hold the page tables

	//
	// Clear all the pages we just requested
	//
	CurDir = (PAGE_ENTRY_AE *)Address;
	for(UINT i = 0; i < 4096; i++)
		CurDir[i].Dummy = 0;

	//
	// Set the kernel part of the address space up
	//
	CurDir[0].Page.Present = 1;
	CurDir[0].Page.Address = ((UINT)KernelMM.DirPointer) >> 12;

	//
	// Set the addresses of the page directories
	//
	UINT ActualAddress;
	for(UINT i = 1; i < 4; i++)
	{
		ActualAddress = (Address + i * PageSize);
		if(KernelMM.GetPhysicalAddress(ActualAddress))
		{
			CurDir[i].Page.Present = 1;
			CurDir[i].Page.Address = ActualAddress >> 12;
		}
		else
			goto TaskPageError;
	}

	//
	// Setup the page directory to point to the page tables
	//	that looks after the memory that contain these pages
	//
	for(UINT i = 2044; i < 2048; i++)
	{
		CurDir[i].Page.Present = 1;
		CurDir[i].Page.Write = 1;
		CurDir[i].Page.User = 1;
		ActualAddress = (Address + ((i - 2040) * PageSize));
		if(KernelMM.GetPhysicalAddress(ActualAddress))
		{
			CurDir[i].Page.Address = ActualAddress >> 12;
		}
		else
			goto TaskPageError;
	}

	//
	// Point the last page tables to ourselves
	//
	for(INT i = 4092; i < 4096; i++)
	{
		CurDir[i].Page.Present = 1;
		CurDir[i].Page.Write = 1;
		CurDir[i].Page.Address = CurDir[i - 2048].Page.Address;
	}

	//
	// Get the physical address of the page directory pointer
	//
	if(KernelMM.GetPhysicalAddress(Address))
	{
		DirPointer = (PAGE_ENTRY_AE *)Address;
	}
	else
		goto TaskPageError;


	//
	// Map in the page directory pointer and page directories
	//
	for(INT i = 2556; i < 2560; i++)
	{
		CurDir[i].Page.Present = 1;
		CurDir[i].Page.Write = 1;
		CurDir[i].Page.Address = Address >> 12;
		Address = CurDir[i - 2555].Page.Address << 12;
	}


	//
	// Unmap the memory we aquired (As should not be accessable from this address space)
	//
	for(UINT vAddress = Address; vAddress < (Address + PageSize * 8); vAddress += PageSize)
		KernelMM.UnMapVirtual(vAddress);

	return;

TaskPageError:
	KernelMM.FreePages(Address, 8);
	//
	// Call a call back function to inform the application
}


VOID *PAGER::GetPages(UINT NumberOfPages, BOOL Kernel)
{

	//
	// Check there are a contiguous number of address
	//	in the address space.
	//
	UINT Count = 0;
	UINT StartDir = NextDirIndex;
	UINT StartPae = NextPaeIndex;

	UINT j = StartPae;
	for(UINT i = StartDir; (i < numDirEntries) && (Count < NumberOfPages); i++)
	{
		//
		// If we have found an already present page
		//	search it
		//
		if(CurDir[i].Page.Present)
		{
			//
			// Get the address of the page table
			//
			CurPae = (PAGE_ENTRY_AE *)(i * 0x1000 + AddressOfPages);
			
			//
			// Search the page table, break if
			//	we have reached our quota of pages
			//
			for(; (j < 512) && (Count < NumberOfPages); j++)
			{
				//
				// If not present its avaliable
				//
				if(!CurPae[j].Page.Present)
				{
					Count++;
				}
				else
				{
					//
					// If it is present we have to
					//	start our search again
					//
					Count = 0;
					if(j == 511)
					{	// Re-set the starting points
						StartDir = i + 1;
						StartPae = 0;
					}
					else
						StartPae = j + 1;
				}
			}

			//
			// re-set our page counter.
			//	This is done here as we may have
			//	started half way through a page
			//	table.
			//
			j = 0;
		}
		else
		{
			//
			// A whole directory is free.
			//
			Count += 512;
			j = 0;
		}
	}

	//
	// Is there room in the address space?
	//
	if(Count < NumberOfPages)
		return 0;

	//
	// Set our loop variables
	//
	UINT RealAddress;
	Count = 0;
	UINT i = StartDir;
	for(j = StartPae; (i < numDirEntries) && (Count < NumberOfPages); i++)
	{
		if(!CurDir[i].Page.Present)
		{
			if(!RealMem.GetPage(RealAddress))
			{
				goto OutOfMemory;
			}
			//
			// Setup the directory to be avaliable
			//
			CurDir[i].Page.Write = 1;
			CurDir[i].Page.Address = RealAddress >> 12;
			if(NextDirPointer > 0)
				CurDir[i].Page.User = 1;	//User is always 1 for PD, as page tables will be user or kernel

			//
			// Set the appropriate index to be avaliable
			//
			PageTablesTable[i].Page.Present = 1;
			PageTablesTable[i].Page.Write = 1;
			PageTablesTable[i].Page.Address = CurDir[i].Page.Address;
			if(NextDirPointer == 0)
				PageTablesTable[i].Page.Global = 1;

			//
			// Get the address of the new page table
			//	and blank it.
			//
			CurPae = (PAGE_ENTRY_AE *)(i * 0x1000 + AddressOfPages);
			for(UINT x = 0; x < 512; x++)
			{
				CurPae[x].Dummy = 0;
			}

			//
			// Set the directory entry so its ready to go
			//
			CurDir[i].Page.Present = 1;
		}
		else
			CurPae = (PAGE_ENTRY_AE *)(i * 0x1000 + AddressOfPages);
		
		//
		// Run through the page tables setting the pages
		//	to present and filling them (We don't need
		//	to check as we know they are all blank to
		//	NumberOfPages)
		//
		for(; j < 512, Count < NumberOfPages; j++)
		{
			if(!RealMem.GetPage(RealAddress))
			{
				goto OutOfMemory;
			}
			CurPae[j].Page.Present = 1;
			CurPae[j].Page.Write = 1;
			CurPae[j].Page.Address = RealAddress >> 12;
			if(!Kernel)	// Check for user or kernel access
				CurPae[j].Page.User = 1;
			else if(NextDirPointer == 0)
				CurPae[j].Page.Global = 1;

			Count++;
		}

		//
		// Save j for below, incase we are finished
		//
		RealAddress = j;
		j = 0;
	}

	//
	// Update our base index's to speed up searching
	//
	if((StartDir == NextDirIndex) && (StartPae == NextPaeIndex))
	{
		//
		// Looks at the loop values to work out the
		//	index's we're actually at.
		//
		NextDirIndex = i - 1;
		if(RealAddress >= 512)
			NextPaeIndex = 0;
		else
			NextPaeIndex = RealAddress;
	}

	//
	// Calculate the starting address of the memory we just allocated
	//
	return (VOID *)(baseAddress + (StartDir * 0x200000) + (StartPae * 0x1000));

OutOfMemory:
	
	//
	// If we are out of memory, we free what we have
	//	allocated so far.
	//
	FreePages((baseAddress + (StartDir * 0x200000) + (StartPae * 0x1000)), Count);
	return 0;
}


VOID PAGER::FreePages(UINT VirtualAddress, UINT NumberOfPages)
{
	//
	// Calculate start and ending page indexes
	//
	VirtualAddress -= baseAddress;
	UINT StartDir = VirtualAddress / 0x200000;
	UINT StartPae = VirtualAddress - (StartDir * 0x200000);
	StartPae /= 0x1000;

	UINT EndDir = NumberOfPages / 512 + StartDir;
	UINT EndPae = NumberOfPages - ((NumberOfPages / 512) * 512) + StartPae;

	//
	// Update our pagers initial search location
	//
	if(NextDirIndex > StartDir)
	{
		NextDirIndex = StartDir;
		NextPaeIndex = StartPae;
	}
	else if((NextDirIndex == StartDir) && (NextPaeIndex > StartPae))
	{
		NextPaeIndex = StartPae;
	}


	//
	// Loop through and free pages
	//
	for(; StartDir <= EndDir; StartDir++)
	{
		CurPae = (PAGE_ENTRY_AE *)(StartDir * 0x1000 + AddressOfPages);	// Page table directory
		for(; (StartPae < 512) && !((StartDir >= EndDir) && (StartPae >= EndPae)); StartPae++)
		{
			//
			// Check page is present,
			//	and free the memory
			//
			if(CurPae[StartPae].Page.Present)
			{
				RealMem.FreePage(CurPae[StartPae].Page.Address << 12);
				CurPae[StartPae].Dummy = 0;
			}
		}
	}
}


BOOL PAGER::GetVirtualAddress(UINT &PhysicalAddress)
{

	//
	// Run through all the entries in the directory
	//
	for(UINT i = 0; i < numDirEntries; i++)
	{
		//
		// If we find a page that is avaliable
		//
		if(!CurDir[i].Page.Present)
			continue;
		
		//
		// Check its not using 4mb pages
		//
		if(CurDir[i].Page.PageSize)
			if(((i+1) * 0x200000) > PhysicalAddress) //4mb pages are virtual to physical
				return true;
			else
				continue;

		//
		// Because all the page directories are 
		//	stored at the top of memory in the same
		//	location we can calculate its virtual
		//	address.
		//
		CurPae = (PAGE_ENTRY_AE *)(i * 0x1000 + AddressOfPages);
		

		//
		// Search a page directory for the page
		//
		for(UINT j = 0; j < 512; j++)
		{
			if(CurPae[j].Page.Present && (((UINT)CurPae[j].Page.Address << 12) == PhysicalAddress))
			{
				//
				// Calculate virtual address based on page indexes
				PhysicalAddress = i * 0x200000 + j * 0x1000 + baseAddress;
				return true;
			}
		}
	}

	return false;
}


BOOL PAGER::GetPhysicalAddress(UINT &VirtualAddress)
{
	//
	// Work out the index in the page tables of the
	//	address we are looking for
	//

	UINT VAddress = VirtualAddress - baseAddress;
	UINT DirEntry = VAddress / 0x200000;
	UINT PaeEntry = VAddress - (DirEntry * 0x200000);
	PaeEntry /= 0x1000;

	//
	// Check the directory entry exists
	//
	if(CurDir[DirEntry].Page.Present && !CurDir[DirEntry].Page.PageSize)
	{
		//
		// Get the virtual address of the page entry
		//
		CurPae = (PAGE_ENTRY_AE *)(DirEntry * 0x1000 + AddressOfPages);
		if(CurPae[PaeEntry].Page.Present)
		{
			//
			// Find the Physical address
			//
			VirtualAddress = CurPae[PaeEntry].Page.Address << 12;
			return true;
		}
	}

	return false;
}


//
// These functions should not be used lightly
//	Will map any physical address to the next avaliable
//	virtual address
//
UINT PAGER::MapPysicalToVirtual(UINT PhysicalAddress)
{
	UINT VirtualAddress = (UINT)GetPages(1);

	if(VirtualAddress != 0)
	{
		UINT VAddress = VirtualAddress - baseAddress;
		UINT DirEntry = VAddress / 0x200000;
		UINT PaeEntry = VAddress - (DirEntry * 0x200000);
		PaeEntry /= 0x1000;

		CurPae = (PAGE_ENTRY_AE *)(DirEntry * 0x1000 + AddressOfPages);
		RealMem.FreePage(CurPae[PaeEntry].Page.Address << 12);
		CurPae[PaeEntry].Page.Address = PhysicalAddress >> 12;
	}

	return VirtualAddress;
}


VOID PAGER::UnMapVirtual(UINT VirtualAddress)
{
	if(VirtualAddress != 0)
	{
		VirtualAddress -= baseAddress;
		UINT DirEntry = VirtualAddress / 0x200000;
		UINT PaeEntry = VirtualAddress - (DirEntry * 0x200000);
		PaeEntry /= 0x1000;

		CurPae = (PAGE_ENTRY_AE *)(DirEntry * 0x1000 + AddressOfPages);
		CurPae[PaeEntry].Dummy = 0;

		if(NextDirIndex <= DirEntry)
		{
			NextDirIndex = DirEntry;
			if(NextPaeIndex < PaeEntry)
				NextPaeIndex = PaeEntry;
		}
	}
}



PAGER::PHYSMEM::PHYSMEM()
{
	//
	// Re-set all the variables
	//
	Length = 0;
	for(int i = 0; i < 32768; i++)
	{
		MemUsage[i] = 0xFFFFFFFF;
	}
	AvaliablePhysicalPages = 0;

	//
	// This variables indicates the start of the kernel
	//	All the free memory before the kernel is used for
	//	DMA devices.
	//
	IndexStart = 8;
}


VOID PAGER::PHYSMEM::SetMemory()
{
	//
	// We add the memory map indexes we are not using
	//	to the kernel heap.
	//
	HAL::KernelMm->AddToHeap((UINT)&MemUsage[Length], (UINT)(&MemUsage[32767] - &MemUsage[Length]));
}


BOOL PAGER::PHYSMEM::GetPage(UINT &Address)
{
	//
	// Returns the highest possible free physical
	//	memory address avaliable, and marks it as used.
	//
	INT i;
	for(i = Length - 1; i >= 0; i--)	// Run backwards through memory
	{
		//
		// Look for a block with a free page
		//
		if(MemUsage[i] != 0xFFFFFFFF)
		{
			ULONG Usage = ~MemUsage[i];
			//
			// Run forwards through this block,
			//	This causes memory fragmentation (Not that it matters as we are using virtual addresses)
			//
			for(UINT j = 0; j < 32; j++)
			{
				//
				// If we find a page, mark it as used
				//	and return the address.
				//
				if(Usage & (1 << j))
				{
					Address = (i * 32 + j) << 12;
					MemUsage[i] ^= 1 << j;
					AvaliablePhysicalPages--;
					return true;
				}
			}
		}
	}

	return false;
}


BOOL PAGER::PHYSMEM::GetLowPage(UINT &Address)
{
	//
	// Same as above except runs forward through memory
	//	up to the first mb.
	//
	UINT i;
	for(i = 0; i < IndexStart; i++)	//First MB
	{
		if(MemUsage[i] != 0xFFFFFFFF)
		{
			ULONG Usage = ~MemUsage[i];
			for(UINT j = 0; j < 32; j++)
			{
				if(Usage & (1 << j))
				{
					Address = (i * 32 + j) << 12;
					MemUsage[i] ^= 1 << j;
					AvaliablePhysicalPages--;
					return true;
				}
			}
		}
	}

	return false;
}


VOID PAGER::PHYSMEM::FreePage(UINT Address)
{
	//
	// Frees a block of physical memory.
	//
	Address = Address >> 12;
	UINT Index = Address / 32;
	Address = Address - Index * 32;

	if(MemUsage[Index] & (1 << Address))
		MemUsage[Index] ^= 1 << Address;
}


VOID PAGER::PHYSMEM::MapMemory(ULONGLONG Address, ULONGLONG MemLength, ULONG Type)
{
	UINT AlignedAddress;
	UINT EndAddress;

	//
	// Rounds the memory map so that only whole 
	//	pages of free memory and marked as
	//	usable.
	//
	if(Type == 1)
	{
		//
		// We only need to bother with usable pages
		//
		AlignedAddress = (UINT)Address;
		if(AlignedAddress % 0x1000 != 0)
			AlignedAddress += 0x1000;
		AlignedAddress = AlignedAddress >> 12;

		EndAddress = (UINT)((Address + MemLength) >> 12);
	}
	else
		return;

	//
	// Get bitmap index
	//
	UINT IndexStart = AlignedAddress / 32;
	UINT IndexEnd = EndAddress / 32;



	//
	// Get bit in that index
	//
	AlignedAddress -= IndexStart * 32;
	EndAddress -= IndexEnd * 32;

	//
	// Set every bit inbetween these two as avaliable
	//	Update Avaliable pages indicator
	//
	for(; IndexStart <= IndexEnd; IndexStart++)
	{
		if((IndexStart != IndexEnd) && (AlignedAddress == 0))
		{
			MemUsage[IndexStart] = 0;
			AvaliablePhysicalPages += 32;
		}
		else
		{
			for(; (AlignedAddress < 32) && !((IndexStart == IndexEnd) && (AlignedAddress == EndAddress)); AlignedAddress++, AvaliablePhysicalPages++)
			{
				MemUsage[IndexStart] ^= 1 << AlignedAddress;
			}

			AlignedAddress = 0;
		}
	}

	//
	// Calculate the memory map index to start searching
	//
	if(Length < IndexEnd)
		if(EndAddress == 0)
			Length = IndexEnd;
		else
			Length = IndexEnd + 1;

/* Code for page boundaries of non-usable memory (not needed)
		AlignedAddress = (UINT)Address >> 12;

		EndAddress = (UINT)(Address + Length);
		if(EndAddress % 0x1000 != 0)
			EndAddress += 0x1000;
		EndAddress = EndAddress >> 12;
*/
}
