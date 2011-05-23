#include "vect.h"


void setvect(void interrupt (far *fp)(),int intno)
{
/*
   home made program to set a vector at specified
   interrupt vector table
*/
   struct addr{
      unsigned offset;
      unsigned base;
      };
   union{
      void interrupt(far *function)();
      struct addr function_addr;
      }intrpt;

   unsigned far * vet_table =(unsigned far *)(4*intno);
   intrpt.function = fp;
   disable();
   *vet_table     = intrpt.function_addr.offset;
   *(vet_table+1) = intrpt.function_addr.base;
   enable();
}

/*___________________________________________________________________*/

void interrupt (far * getvect(int intno))()
{
/*
   home made program to get a vector from specified
   interrupt vector table
*/
   struct addr{
      unsigned offset;
      unsigned base;
      };
   union{
      void interrupt(far *function)();
      struct addr function_addr;
      }intrpt;

   unsigned far * vet_table =(unsigned far *)(4*intno);
   disable();
   intrpt.function_addr.offset=*vet_table    ;
   intrpt.function_addr.base  =*(vet_table+1)  ;
   enable();
   return(intrpt.function);
}