#ifndef __MEMORYCONTROL_H__
#define __MEMORYCONTROL_H__


#include <def.h>


class MEMORYCONTROL
{
	protected:
		MEMORYCONTROL();
		~MEMORYCONTROL();

		static UINT PageSize;

		static MEMORYCONTROL *Head;
		static MEMORYCONTROL *Tail;
		MEMORYCONTROL *Next;
		MEMORYCONTROL *Previous;
};


class PAGING: public virtual MEMORYCONTROL
{
	public:
		virtual VOID *GetPages(UINT StartingAddress, BOOL Kernel) = 0;
		virtual VOID FreePages(UINT VirtualAddress, UINT NumberOfPages) = 0;

	protected:
		PAGING(){};
};


#endif


