#ifndef _A20_H_
#define _A20_H_

#define A20_BIT 0x0200

extern unsigned short int GET_A20_STATE ( void ) ; /*returns a20 bit*/
extern void A20_ENABLE ( void ) ; /*enables the A20 gate*/
extern void A20_DISABLE ( void ) ; /*Disables the gate*/

#endif
