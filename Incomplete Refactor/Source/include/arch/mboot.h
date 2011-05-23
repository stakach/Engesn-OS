#ifndef __MBOOT_H__
#define __MBOOT_H__


#include <def.h>


//
// Mboot header found in 'kstub.asm'.
//

struct MULTIBOOT_HEADER
{
  ULONG Magic;
  ULONG Flags;
  ULONG Checksum;
  ULONG HeaderAddr;
  ULONG LoadAddr;
  ULONG LoadEndAddr;
  ULONG BssEndAddr;
  ULONG EntryAddr;
} __attribute__ ((packed));


//
// The symbol table for a.out.
//

struct AOUT_SYMBOL_TABLE
{
  ULONG TabSize;
  ULONG StrSize;
  ULONG Addr;
  ULONG Reserved;
} __attribute__ ((packed));


//
// The section header table for ELF.
//

struct ELF_SECTION_HEADER_TABLE
{
  ULONG Num;
  ULONG Size;
  ULONG Addr;
  ULONG Shndx;
} __attribute__ ((packed));


//
// The Multiboot information.
//

struct MULTIBOOT_INFO
{
  ULONG Flags;
  ULONG MemLower;
  ULONG MemUpper;
  ULONG BootDevice;
  ULONG CmdLine;
  ULONG ModsCount;
  ULONG ModsAddr;
  union
  {
    AOUT_SYMBOL_TABLE			AoutSym;
    ELF_SECTION_HEADER_TABLE	ElfSec;
  } U;
  ULONG MmapLength;
  ULONG MmapAddr;
} __attribute__ ((packed));


//
// The module structure.
//

struct MODULE
{
  ULONG ModStart;
  ULONG ModEnd;
  ULONG String;
  ULONG Reserved;
} __attribute__ ((packed));


//
// The memory map. Be careful that the offset 0 is BaseAddrLow
//	but no size.
//

struct MEMORY_MAP
{
  ULONG Size;
  ULONGLONG BaseAddr;
  ULONGLONG Length;
  ULONG Type;
} __attribute__ ((packed));


extern const MULTIBOOT_HEADER	*MbootHeader;
extern const MULTIBOOT_INFO		*MbootInfo;


namespace I386
{


	KE_STATUS CheckMboot();


}


#endif
