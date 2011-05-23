#include "keyboard.h"

unsigned char leds;			/*The leds that are on and off on the keyboard*/

#define MAX_TRYES_FOR_READY 1000000L
#define MAX_TRYES_TO_SEND_DATA 4
#define MAX_TRYES_TO_RESPONSE  2000000L


void waitKey()                           /* Wait for a keypress          */
{
	uchar kbc_status;                    /* The KBC status               */
	kbFlushBuffer();
	do                                   /* Loop until a key is pressed */
	{                                    /* Including Shift, Ctrl etc.   */
		while(inportb(0x64) & 0x02);     /* Get KBC status               */
		kbc_status = inportb(0x64);
	}                                    /* Continue until bit 0 is set  */
	while(!(kbc_status & 1));            /* Very primitive, but it works */
}


char getch()
{
	unsigned int headptr;         /* pointer to head of buffer */
	unsigned int tailptr;         /* pointer to tail of buffer */
	unsigned int key;

	while(1) 
	{
		/* the BIOS holds the head pointer at 0040:001Ah &
	      holds the tail pointer at 0040:001Ch              */
	    headptr = *(unsigned int far *) 0x0040001A;
		tailptr = *(unsigned int far *) 0x0040001C;

	    /* If the two pointers are different, then there is a key
	       in this buffer ready for extraction
	       Buffer is in the 0040h segment with an offset of HEADPTR (above) */
		if(headptr != tailptr)
		{
			/*key = *(unsigned int far *) (0x00400000 + headptr);*/
			key = bios_getch();
			break;
		}
	}
	return key;
}


void kbFlushBuffer() /*Clears the keyboard buffer*/
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




void kbWaitForReady(void) /*determines if the keyboard is ready to accept a command*/
{
 	volatile long i = MAX_TRYES_FOR_READY;
 	while (i-- && (inportb(0x64) & 0x02));
}




void kbSendCommand(unsigned char c) /*Sends a command to the keyboard*/
{
 	kbWaitForReady();
 	outportb(0x64,c);
}




void kbSendData(unsigned char data) /*Sends a command or data to keyboard*/
{
	kbWaitForReady();   /*MicroDelay((unsigned long) 10);*/
	outportb(0x60, data);
}




void kbUpdateLeds(void) /*Updates the keyboard leds*/
{
	kbSendData(0xED);
	kbSendData(leds);
}




void kbSetRate(unsigned char rate) /*Sets the typing rate and delay*/
{
	kbSendData(0xF3);
	kbSendData(rate);
}




void kbSetup()
{
	kbSendData(0xFF);	/*Restart keyboard*/
	kbSendData(0xF4);	/*Enables keyboard and Scanning*/
	kbSetRate(KDefault);  /*Sets default scanning rate*/
	kbUpdateLeds();		  /*Makes sure no leds are on*/
}

