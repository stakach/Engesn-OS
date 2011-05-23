#ifndef _asmf_H_
#define _asmf_H_
#include "kerndefs.h"

/* Assembly routines */
/*********************/
extern void outportb( word , byte ) ; /*moves byte to a memory location*/
extern void outportw( word , word ) ; /*moves word to a memory location*/
extern byte inportb( word ) ; /*returns byte from memory location*/
extern word inportw( word ) ; /*returns word from memory location*/


extern void enable( ); /*enables interupts*/
extern void disable( ); /*disables interupts*/
extern void halt( ); /*Halts cpu*/


/*---------------*/
/*bios functions:*/
/*---------------*/
extern uchar bios_getch(); /*gets keyboard input*/
extern unsigned int bios_getscan(); /*gets keyboard input*/
extern void MicroDelay(ulong MicroSecs); /*delays execution for a period of time*/

#endif
