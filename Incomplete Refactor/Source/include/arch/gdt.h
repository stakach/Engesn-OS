#ifndef __GDT_H__
#define __GDT_H__


#include <def.h>
#include <arch/i386.h>


#define MAX_GDT 32


namespace I386
{


	class GDT
	{
		public:
			X86_SELECTOR AddDesc(ULONG base, ULONG limit, UINT opt);
			VOID RemDesc(const X86_SELECTOR &selector);

			static GDT gdt;

		private:
			GDT();
			UINT FindEmpty();

			static DT_ENTRY Gdt[MAX_GDT];
	};


}


#endif
