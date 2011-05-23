#ifndef __PAGING_H__
#define __PAGING_H__


#include <def.h>
#include <hal.h>


//
// This is the structure to use for standard x86 paging + 4mb paging
//	with page size extensions enabled in CR4, and page size = 1. (0x10 | CR4)
//

union PAGE_ENTRY
{
	struct
	{
		ULONG Present	:	1;	// Is the page present?
		ULONG Write		:	1;	// Is the page writeable?
		ULONG User		:	1;	// Are we in user mode?
		ULONG WriteT	:	1;	// Page Write Through
		ULONG CacheD	:	1;	// Page Cache Disable
		ULONG Accessed	:	1;
		ULONG Drity		:	1;
		ULONG PageSize	:	1;	// Only avaliable in page directories
		ULONG Global	:	1;
		ULONG Other		:	3;	// Free for general use
		ULONG Address	:	20;	// Address of page \ page table
	}Page;
	ULONG Dummy;
} __attribute__ ((packed));


//
// This is the structure to use for extended addressing x86 paging.
//	With page adress extensions enabled in CR4. (0x20 | CR4)
//	2mb pages can be enabled with page size = 1.
//	Also includes 36bit address space.
//

union PAGE_ENTRY_AE
{
	struct
	{
		ULONG Present	:	1;	// Is the page present?
		ULONG Write		:	1;	// Is the page writeable?
		ULONG User		:	1;	// Are we in user mode?
		ULONG WriteT	:	1;	// Page Write Through
		ULONG CacheD	:	1;	// Page Cache Disable
		ULONG Accessed	:	1;
		ULONG Drity		:	1;
		ULONG PageSize	:	1;	// Only avaliable in page directories
		ULONG Global	:	1;	// Only avaliable in page table entries
		ULONG Other		:	3;	// Free for general use
		ULONG Address	:	24;	// Address of page \ page table
		ULONG Reserved	:	28;
	}Page;
	ULONGLONG Dummy;
} __attribute__ ((packed));


#define USER 1


namespace I386
{


	class PAGER: public MEMORY
	{
		public:
			PAGER(BOOL UserMode);
			~PAGER(){}
			virtual VOID *GetPages(UINT NumberOfPages, BOOL Kernel = false);
			virtual VOID FreePages(UINT VirtualAddress, UINT NumberOfPages);
			VOID SetMemory();
			BOOL GetPhysicalAddress(UINT &VirtualAddress);
			UINT GetKmemVersion();

			static PAGER KernelMM;

			class PHYSMEM
			{
				public:
					VOID MapMemory(ULONGLONG Address, ULONGLONG MemLength, ULONG Type);
					BOOL GetLowPage(UINT &Address);

				private:
					friend class PAGER;

					PHYSMEM();
					BOOL GetPage(UINT &Address);
					VOID FreePage(UINT Address);
					VOID SetMemory();

					UINT AvaliablePhysicalPages;
					ULONG Length;
					ULONG IndexStart;
					ULONG MemUsage[32768];
			};
			static PHYSMEM RealMem;
			
		private:
			BOOL GetVirtualAddress(UINT &PhysicalAddress, PAGE_ENTRY *Dir = 0);
			UINT MapPysicalToVirtual(UINT PhysicalAddress);
			VOID UnMapVirtual(UINT VirtualAddress);

			INT NextDirIndex;
			INT NextPaeIndex;
			UINT KMemVersion;
			PAGE_ENTRY *Directory;
			PAGE_ENTRY *VDirectory;
			PAGE_ENTRY *Temp;

			PAGER();
	};


}


#endif

