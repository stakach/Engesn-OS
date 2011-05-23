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
	KMemVersion = 0;			// Initial Memory Map (We use versioning to map changes accross address spaces this does not effect kmemory as all pages are marked as global)
	put_cr4(get_cr4() | 0x10 | 0x20 | 0x80);	// Enable Page Size Extensions (2mb Pages), Paging extentions (0x20 bit 5) and Global pages (0x80 bit 7)
}


VOID PAGER::SetMemory()
{
	RealMem.SetMemory();	// Inform RealMem we have finished mapping the memory

	//
	// Setup virtual memory for the kernel
	//
	UINT DirAddress, PaeAddress;
	if(RealMem.GetPage(DirAddress) && RealMem.GetPage(PaeAddress))
	{
		Directory = (PAGE_ENTRY *)DirAddress;

		//
		// Clear page directory (first 63 are kernel)
		//
		int i = 0;
		for(; i < 1024; i++)
		{
			Directory[i].Dummy = 0;
		}

		//
		// Setup page directory with 4mb pages,
		//	mapping the current kernel into memory
		//
		i = 0;
		int j = 0;
		int k = 8;
		for(; i < (ULONG)&end; j++, i += 0x400000)
		{
			Directory[j].Page.Present = 1;
			Directory[j].Page.Write = 1;
			Directory[j].Page.PageSize = 1;
			Directory[j].Page.Address = i >> 12;

			//
			// Mark the memory used by the kernel
			//	There is some excess, this becomes the kernel Heap
			//
			for(; k < (j * 32); k++)
			{
				RealMem.MemUsage[k] = 0xFFFFFFFF;
			}
		}

		//
		// Tell the kernel where the next free entries are
		//
		if(Directory[j].Page.Present == 0)
			NextDirIndex = j;
		else
			NextDirIndex = j + 1;
		NextPaeIndex = 0;

		//
		// Add the excess to the heap
		//
		HAL::KernelMm->AddToHeap((UINT)&end, (UINT)(i - 0x400000 - (UINT)&end));

		//
		// Add the page table to kernel virtual space
		//
		Directory[1023].Page.Present = 1;
		Directory[1023].Page.Write = 1;
		Directory[1023].Page.Address = PaeAddress >> 12;

		//
		// Empty the page table
		//
		Temp = (PAGE_ENTRY *)PaeAddress;
		for(i = 0; i < 1024; i++)
		{
			Temp[i].Dummy = 0;
		}

		//
		// Add the page directory and page table to the
		//	Virtual Address space (Kind of crazy)
		//
		Temp[0].Page.Present = 1;
		Temp[0].Page.Write = 1;
		Temp[0].Page.Address = DirAddress >> 12;

		//
		// Map the page to the allocated address space
		//
		Temp[1023].Page.Present = 1;
		Temp[1023].Page.Write = 1;
		Temp[1023].Page.Address = PaeAddress >> 12;


		//
		// Put page directory into cr3, then enable paging in cr0.
		//
		put_cr3((ULONG)DirAddress);
		put_cr0(get_cr0() | 0x80000000);	// Enable Paging.

		//
		// Get the virtual (Accessable to us, address of the directory)
		//
		VDirectory = (PAGE_ENTRY *)0xFFC00000;
	}
	else
		I386HAL::SystemHal.CriticalException("Paging initialisation error", 36, 0);
}


PAGER::PAGER(BOOL UserMode)
{


	
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


VOID *PAGER::GetPages(UINT NumberOfPages, BOOL Kernel)
{

	//
	// Check there are a contiguous number of address
	//	in the address space.
	//
	BOOL VerChangeRequired = false;
	UINT Count = 0;
	UINT StartDir = NextDirIndex;
	UINT StartPae = NextPaeIndex;
	UINT j = StartPae;
	for(UINT i = StartDir; (i < 1023) && (Count < NumberOfPages); i++)
	{
		//
		// If we have found an already present page
		//	search it
		//
		if(VDirectory[i].Page.Present)
		{
			//
			// Get the address of the page table
			//
			Temp = (PAGE_ENTRY *)(i * 0x1000 + 0xFFC00000);
			
			//
			// Search the page table, break if
			//	we have reached our quota of pages
			//
			for(; (j < 1024) && (Count < NumberOfPages); j++)
			{
				//
				// If not present its avaliable
				//
				if(!Temp[j].Page.Present)
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
					if(j == 1023)
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
			Count += 1024;
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
	for(j = StartPae; (i < 1023) && (Count < NumberOfPages); i++)
	{
		if(!VDirectory[i].Page.Present)
		{
			if(!RealMem.GetPage(RealAddress))
			{
				goto OutOfMemory;
			}
			//
			// Setup the directory to be avaliable
			//
			VDirectory[i].Page.Write = 1;
			VDirectory[i].Page.Address = RealAddress >> 12;
			VDirectory[i].Page.User = 1;	//User is always 1 for PD, as page tables will be user or kernel

			//
			// Get the paging page table address
			//
			Temp = (PAGE_ENTRY *)(1023 * 0x1000 + 0xFFC00000);

			//
			// Set the appropriate index to be avaliable
			//
			Temp[i].Page.Present = 1;
			Temp[i].Page.Write = 1;
			Temp[i].Page.Address = VDirectory[i].Page.Address;
			if(i < 64)
			{
				Temp[i].Page.Global = 1;
				VerChangeRequired = true;
			}

			//
			// Get the address of the new page table
			//	and blank it.
			//
			Temp = (PAGE_ENTRY *)(i * 0x1000 + 0xFFC00000);
			for(UINT x = 0; x < 1024; x++)
			{
				Temp[x].Dummy = 0;
			}

			//
			// Set the directory entry so its ready to go
			//
			VDirectory[i].Page.Present = 1;
		}
		else
			Temp = (PAGE_ENTRY *)(i * 0x1000 + 0xFFC00000);
		
		//
		// Run through the page tables setting the pages
		//	to present and filling them (We don't need
		//	to check as we know they are all blank to
		//	NumberOfPages)
		//
		for(; j < 1024, Count < NumberOfPages; j++)
		{
			if(!RealMem.GetPage(RealAddress))
			{
				goto OutOfMemory;
			}
			Temp[j].Page.Present = 1;
			Temp[j].Page.Write = 1;
			Temp[j].Page.Address = RealAddress >> 12;
			if(!Kernel)	// Check for user or kernel access
				Temp[j].Page.User = 1;
			else if(i < 64)
			{
				Temp[j].Page.Global = 1;
				VerChangeRequired = true;
			}

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
		if(RealAddress >= 1024)
			NextPaeIndex = 0;
		else
			NextPaeIndex = RealAddress;
	}

	if(VerChangeRequired)
		KMemVersion++;

	//
	// Calculate the starting address of the memory we just allocated
	//
	return (VOID *)((StartDir * 0x400000) + (StartPae * 0x1000));

OutOfMemory:
	
	//
	// If a version change was required we subtract a version
	//	(As free pages will want to do one and we're
	//	Undoing what we just changed) <-- BUGBUG::Not entirely true
	//	as free pages won't delete the page tables we've allocated
	//	space for, this needs to be done!
	//
	if(VerChangeRequired)
		KMemVersion--;

	//
	// If we are out of memory, we free what we have
	//	allocated so far.
	//
	FreePages((StartDir * 0x400000) + (StartPae * 0x1000), Count);
	return 0;
}


VOID PAGER::FreePages(UINT VirtualAddress, UINT NumberOfPages)
{
	//
	// Calculate start and ending page indexes
	//
	BOOL VerChangeRequired = false;
	UINT StartDir = VirtualAddress / 0x400000;
	UINT StartPae = VirtualAddress - (StartDir * 0x400000);
	StartPae /= 0x1000;

	UINT EndDir = NumberOfPages / 1024 + StartDir;
	UINT EndPae = NumberOfPages - ((NumberOfPages / 1024) * 1024) + StartPae;

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
		Temp = (PAGE_ENTRY *)(StartDir * 0x1000 + 0xFFC00000);	// Page table directory
		for(; (StartPae < 1024) && !((StartDir >= EndDir) && (StartPae >= EndPae)); StartPae++)
		{
			//
			// Check page is present,
			//	and free the memory
			//
			if(Temp[StartPae].Page.Present)
			{
				RealMem.FreePage(Temp[StartPae].Page.Address << 12);
				Temp[StartPae].Dummy = 0;
				if(StartDir < 64)
					VerChangeRequired = true;
			}
		}
	}

	//
	// If we have modified kernel memory inform task switcher
	//
	if(VerChangeRequired)
		KMemVersion++;
}


BOOL PAGER::GetVirtualAddress(UINT &PhysicalAddress, PAGE_ENTRY *Dir)
{
	//
	//	NOTE:: This function could fail with low pages < 1mb
	// Are we looking at our directory or someone elses
	//
	if(Dir == NULL)
		Dir = VDirectory;

	//
	// Run through all the entries in the directory
	//
	for(UINT i = 0; i < 1024; i++)
	{
		//
		// If we find a page that is avaliable
		//
		if(!Dir[i].Page.Present)
			continue;
		
		//
		// Check its not using 4mb pages
		//
		if(Dir[i].Page.PageSize)
			if(((i+1) * 0x400000) > PhysicalAddress) //4mb pages are virtual to physical
				return true;
			else
				continue;

		//
		// Because all the page directories are 
		//	stored at the top of memory in the same
		//	location we can calculate its virtual
		//	address.
		//
		Temp = (PAGE_ENTRY *)(i * 0x1000 + 0xFFC00000);
		

		//
		// Search a page directory for the page
		//
		for(UINT j = 0; j < 1024; j++)
		{
			if(Temp[j].Page.Present && (((UINT)Temp[j].Page.Address << 12) == PhysicalAddress))
			{
				//
				// Calculate virtual address based on page indexes
				PhysicalAddress = i * 0x400000 + j * 0x1000;
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
	UINT DirEntry = VirtualAddress / 0x400000;
	UINT PaeEntry = VirtualAddress - (DirEntry * 0x400000);
	PaeEntry /= 0x1000;

	//
	// Check the directory entry exists
	//
	if(VDirectory[DirEntry].Page.Present && !VDirectory[DirEntry].Page.PageSize)
	{
		//
		// Get the virtual address of the page entry
		//
		Temp = (PAGE_ENTRY *)(DirEntry * 0x1000 + 0xFFC00000);
		if(Temp[PaeEntry].Page.Present)
		{
			//
			// Find the virtual address
			//
			VirtualAddress = Temp[PaeEntry].Page.Address << 12;
			return true;
		}
	}

	return false;
}


UINT PAGER::GetKmemVersion()
{
	return KMemVersion;
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
		UINT DirEntry = VirtualAddress / 0x400000;
		UINT PaeEntry = VirtualAddress - (DirEntry * 0x400000);
		PaeEntry /= 0x1000;

		Temp = (PAGE_ENTRY *)(DirEntry * 0x1000 + 0xFFC00000);
		RealMem.FreePage(Temp[PaeEntry].Page.Address << 12);
		Temp[PaeEntry].Page.Address = PhysicalAddress >> 12;
	}

	return VirtualAddress;
}


VOID PAGER::UnMapVirtual(UINT VirtualAddress)
{
	if(VirtualAddress != 0)
	{
		UINT DirEntry = VirtualAddress / 0x400000;
		UINT PaeEntry = VirtualAddress - (DirEntry * 0x400000);
		PaeEntry /= 0x1000;

		Temp = (PAGE_ENTRY *)(DirEntry * 0x1000 + 0xFFC00000);
		Temp[PaeEntry].Dummy = 0;

		NextDirIndex = DirEntry;
		NextPaeIndex = PaeEntry;
	}
}

