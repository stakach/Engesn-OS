#include "keyboard.h"
#include "string.h"

extern Interrupt intcontrol;
extern void keyboardInt();
extern void UpdateLeds(unsigned char led);

extern int buffend;
extern unsigned char scanCode;
extern char buffer[];
extern Func_Key FKey[10];

void Keyboard::waitKey()   /* Wait for a keypress          */
{
	FlushBuffer();
	unsigned char scan = scanCode;
	while(scan == scanCode);
}


char Keyboard::getch()
{
	int i = 0;
	while(buffend == 0);

	disable();

	for(; i < buffend; i++)
	{
		buffer[i] = buffer[i + 1];
	}
	buffend--;

	enable();

	return buffer[0];
}


void Keyboard::FlushBuffer() /*Clears the keyboard buffer*/
{
	unsigned temp;
	do
	{
		temp = inportb(0x64);
		if((temp & 0x01) != 0) 
		{
			(void)inportb(0x60);
			continue;
		}
	} while((temp & 0x02) != 0);
}




void Keyboard::WaitForReady(void) /*determines if the keyboard is ready to accept a command*/
{
 	volatile long i = MAX_TRYES_FOR_READY;
 	while (i-- && (inportb(0x64) & 0x02));
}




void Keyboard::SendCommand(unsigned char c) /*Sends a command to the keyboard*/
{
 	WaitForReady();
 	outportb(0x64,c);
}




void Keyboard::SendData(unsigned char data) /*Sends a command or data to keyboard*/
{
	WaitForReady();   /*MicroDelay((unsigned long) 10);*/
	outportb(0x60, data);
}




void Keyboard::SetRate(unsigned char rate) /*Sets the typing rate and delay*/
{
	SendData(0xF3);
	SendData(rate);
}



bool Keyboard::SetFunctionKey(unsigned int num, void (* function)())
{
	if(0 < num <= 10)
	{
		if(!FKey[num - 1].enabled)
		{
			FKey[num - 1].func = function;
			FKey[num - 1].enabled = true;
			return true;
		}
	}
	return false;
}



Keyboard::Keyboard()
{
	intcontrol.enable_irq(1, keyboardInt, INT_GATE|BITS_32|PRESENT|RING_2);
	SendData(0xFF);	/*Restart keyboard*/
	SendData(0xF4);	/*Enables keyboard and Scanning*/
	SetRate(KDefault);  /*Sets default scanning rate*/
	UpdateLeds(0);		  /*Makes sure no leds are on*/
}



Keyboard::~Keyboard() {}




