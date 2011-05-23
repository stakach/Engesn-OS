#ifndef _string_H_
#define _string_H_

char memcmp(const void *_m1, const void *_m2, unsigned short n);	//compare pecies of memory
void memcpy(void *_d, void *_s, unsigned short n);					//compies memory
unsigned short strlen(char *s);									//calculates the length of a string
void itoa(unsigned int n, unsigned char *s, char base);		//Converts a number to a string
unsigned int strtoul(const char *s, char **ptr, int base);		//Converts a string to a number
int strtol(const char *s, char **ptr, int base);				//Converts a string to a signed number


#endif
