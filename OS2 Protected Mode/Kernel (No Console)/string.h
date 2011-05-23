#ifndef _string_H_
#define _string_H_

#include "kerndefs.h"

char memcmp(const void *_m1, const void *_m2, size_t n); /*compare pecies of memory*/
void memcpy(void *_d, void *_s, size_t n); /*compies memory*/
size_t strlen(char *s); /*calculates the length of a string*/
void ntostr(unsigned int i, char *s);


#endif
