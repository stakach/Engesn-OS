#ifndef __HAL_H__
#define __HAL_H__


#include <def.h>
#include <memorycontrol.h>
#include <heap.h>


class MEMORY: public HEAP, public PAGING
{
	public:
		MEMORY(){}
		~MEMORY(){}

	protected:

};


class DEBUG_CONSOLE
{
	public:
		virtual VOID Clear() = 0;
		virtual VOID Write(CHAR *cString) = 0;
		virtual VOID Write(ULONG Num, UINT Base) = 0;
		virtual VOID Put(const CHAR &c) = 0;
		virtual VOID GotoXY(UINT x, UINT y) = 0;
};


enum SHUTDOWN_TYPE {SHUTDOWN, RESTART};


class HAL
{
	public:
		virtual KE_STATUS Setup() = 0;
		virtual VOID Shutdown(SHUTDOWN_TYPE Type) = 0;
		virtual MEMORY *GetAddressSpace(BOOL UserMode) = 0;

		static DEBUG_CONSOLE *Console;
		static MEMORY *KernelMm;
		static HAL *hal;
};


#endif
