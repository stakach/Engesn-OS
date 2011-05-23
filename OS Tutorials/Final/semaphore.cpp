
/* Engesn (Derived) Operating System
 * Copyright (c) 2002, 2003 Stephen von Takach
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software
 * is hereby granted, provided that both the copyright notice and 
 * this permission notice appear in all copies of the software, 
 * derivative works or modified versions. 
*/


#include "include\semaphore.h"
#include "include\asmf.h"		//enable, disable

namespace
{
	bool enabled;		//state of interrupts before semStart called
	unsigned long control = 0;
}

void semStart()
{
	if(control == 0)
	{					//Save previous state
		enabled = !asmflags::disabled;
	}
	disable();			//Disable interrupts
	control++;			//Add semaphore to control
}

void semEnd()
{
	if(control > 0)
	{
		control--;		//Subtract semaphore from control
		if((control == 0) && enabled)
		{
			enable();	//enable interrupts if they were enabled in previous state
		}
	}
}


