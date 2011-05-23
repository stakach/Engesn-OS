#ifndef __DRIVER_H__
#define __DRIVER_H__


#include <def.h>


enum DRIVER_TYPES {TIMER, KEYBOARD, MOUSE, DISK, DISPLAY, AUDIO, SERIAL};


class DRIVER
{
	public:
		DRIVER(CHAR *Name, UINT Type, UINT Irq);
		~DRIVER();
		virtual VOID EnableIrq() = 0;
		virtual VOID DisableIrq() = 0;

		static DRIVER *GetDriver(UINT Irq);

	protected:
		CHAR *Name;
		UINT Irq;
		UINT Type;

	private:
		static DRIVER *Head;
		static DRIVER *Tail;
		DRIVER *Next;
		DRIVER *Previous;
};


#endif


