//string.cpp

#include "..\include\string.h"
#include "..\include\limits.h"


#ifdef Isspace
#undef Isspace
#endif // Isspace
#define Isspace(c) ((c == ' ') || (c == '\t') || (c=='\n') || (c=='\v') || \
			(c == '\r') || (c == '\f'))


//compares two peices of memory
char memcmp(const void *_m1, const void *_m2, unsigned short n)
{
	unsigned char *m1 = (unsigned char *)_m1, *m2 = (unsigned char *)_m2;
	unsigned short i;

	for(i=0; i<n; i++)
	{
		if(m1[i] != m2[i])
		{
			return (m1[i] > m2[i] ? 1 : -1);
		}
	}
	return 0;
}

//copies a peice of memory to a new location
void memcpy(void *_d, void *_s, unsigned short n)
{
	unsigned char *d = (unsigned char *)_d, *s = (unsigned char *)_s;
	unsigned short i;

	for(i=0; i<n; i++)
	{
		d[i] = s[i];
	}
}

//calculates the length of a string (including '\0')
unsigned short strlen(char *s)
{
	unsigned short len = 0;

	while(*s != '\0')
	{
		*s++;
		len++;
	}

	return len;
}

//Converts a number to a string
void itoa(unsigned int n, unsigned char *s, char base)
{
	unsigned int i, j;
	unsigned char tmp[22], digit;

	i = 0;
	do
	{
		j = n;
		n /= base;		// divide num by base
		n *= base;		// multiply num by b to isolate the digit
		digit = j - n;
		
						// change the numerical digit to printable ASCII value
		if(digit <= 9)
		{
			tmp[i] = digit + '0';
		}
		else
		{
			tmp[i] = digit - 0x0A + 'A';
		};
		n /= base;		// divide num by b again to get to the next digit
		i++;
	} while(n != 0);

	if(base == 16)
	{
		tmp[i] = 'x';
		i++;
		tmp[i] = '0';
		i++;
	}
	
	tmp[i] = '\0';		// add the terminator
	
						// reverse string
	j = strlen((char *)tmp) - 1;
	for (i = 0; i < strlen((char *)tmp); i++) 
	{
		s[i] = tmp[j];
		j--;
	}
	s[i] = '\0';
}

//Converts a string to a number with defined base
unsigned int strtoul(const char *s, char **ptr, int base)
{
	unsigned int total = 0;
	unsigned int digit;
	int radix;
	const char *start = s;
	int did_conversion = 0;
	int overflow = 0;
	int minus = 0;
	unsigned int maxdiv, maxrem;

	if (s == '\0') 
	{
		if (!ptr) 
		{
			*ptr = (char *) start;
		}
		return(0L);
	}

	while (Isspace (*s)) 
	{
		s++;
	}

	if (*s == '-') 
	{
		s++;
		minus = 1;
	} else if (*s == '+') 
	{
		s++;
	}

	radix = base;
	if (base == 0 || base == 16) 
	{
		/*
		 * try to infer radix from the string (assume decimal).
		 * accept leading 0x or 0X for base 16.
		 */
		if (*s == '0') 
		{
			did_conversion = 1;
			if (base == 0) 
			{
				radix = 8;	// guess it's octal
			}
			s++;			// (but check for hex)
			if (*s == 'X' || *s == 'x') 
			{
				did_conversion = 0;
				s++;
				radix = 16;
			}
		}
	}
	if (radix == 0) 
	{
		radix = 10;
	}
	
	maxdiv = ULONG_MAX / radix;
	maxrem = ULONG_MAX % radix;

	while ((digit = *s) != 0) 
	{
		if (digit >= '0' && digit <= '9' && digit < ('0' + radix)) 
		{
			digit -= '0';
		} 
		else if (radix > 10) 
		{
			if (digit >= 'a' && digit < ('a' + radix - 10)) 
			{
				digit = digit - 'a' + 10;
			} 
			else if (digit >= 'A' &&
					digit < ('A' + radix - 10)) 
			{
				digit = digit - 'A' + 10;
			} 
			else 
			{
				break;
			}
		} 
		else 
		{
			break;
		}
		did_conversion = 1;
		if (total > maxdiv || (total == maxdiv && digit > maxrem)) 
		{
			overflow = 1;
		}
		total = (total * radix) + digit;
		s++;
	}
	if (overflow) 
	{
		if (ptr != '\0') 
		{
			*ptr = (char *) s;
		}
		return(ULONG_MAX);
	}
	if (ptr != '\0') 
	{
		*ptr = (char *) ((did_conversion) ? (char *) s : start);
	}
	return(minus ? - total : total);
}

//Converts a string to a signed (possible negative) number with defined base
int strtol(const char *s, char **ptr, int base)
{
	int minus = 0;
	unsigned int tmp;
	const char *start = s;
	char *eptr;

	if (s == '\0') 
	{
		if (!ptr) 
		{
			*ptr = (char *) start;
		}
		return(0L);
	}

	while (Isspace (*s)) 
	{
		s++;
	}
	if (*s == '-') 
	{
		s++;
		minus = 1;
	} else if (*s == '+') 
	{
		s++;
	}

	// Let strtoul do the hard work.

	tmp = strtoul(s, &eptr, base);
	if (ptr != '\0') 
	{
		*ptr = (char *)((eptr == s) ? start : eptr);
	}
	if (tmp > (minus ? - (unsigned long) LONG_MIN : (unsigned long) LONG_MAX)) 
	{
		return(minus ? LONG_MIN : LONG_MAX);
	}
	return(minus ? (long) -tmp : (long) tmp);
}

