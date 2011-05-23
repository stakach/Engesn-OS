#ifndef __HEAP_H__
#define __HEAP_H__


#include <def.h>
#include <memorycontrol.h>


class HEAP: public virtual MEMORYCONTROL
{
	public:
		VOID *Kmalloc(UINT Size);
		VOID Kfree(UINT Address);
		VOID *Krealloc(UINT Address, UINT Size);
		VOID *KmallocA4(UINT Size);
		VOID AddToHeap(UINT Address, UINT Size){};

	private:
		VOID InsertFree(UINT Prev, UINT Address);
		VOID InsertUsed(UINT Prev, UINT Address);
		VOID *MoreCore(UINT &TotalSize);

		VOID *FreeStack;
		VOID *UsedStack;

	protected:
		HEAP(){};
};


#endif


