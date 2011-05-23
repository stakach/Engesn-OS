#ifndef __I386Driver_H__
#define __I386Driver_H__


#include <def.h>
#include <driver.h>


namespace I386
{


	class I386DRIVER: public DRIVER
	{
		public:
			I386DRIVER(CHAR *Name, UINT Type, UINT Irq);
			virtual VOID EnableIrq();
			virtual VOID DisableIrq();
			VOID ClearInterrupt();
	};


}


#endif

