#include "string.h"


char memcmp(void *_m1, void *_m2, size_t n) /*compares two peices of memory*/
{
	uchar *m1 = (uchar *)_m1, *m2 = (uchar *)_m2;
	size_t i;

	for(i=0; i<n; i++)
	{
		if(m1[i] != m2[i])
		{
			return (m1[i] > m2[i] ? 1 : -1);
		}
	}
	return 0;
}

void memcpy(void *_d, void *_s, size_t n) /*copies a peice of memory to a new location*/
{
	uchar *d = (uchar *)_d, *s = (uchar *)_s;
	size_t i;

	for(i=0; i<n; i++)
	{
		d[i] = s[i];
	}
}

size_t strlen(char *s) /*calculates the length of a string*/
{
	size_t len = 0;

	while(*s != '\0')
	{
		*s++;
		len++;
	}

	return len;
}

void ntostr(unsigned int i, char *s)
{
   unsigned char backstr[21], j=0, l=0, m=0;

   do					/* Convert string one digit at	*/
   {					/* a time			*/
      backstr[j++] = (i % 10) + '0';	/* Put a digit in backstr	*/
      i /= 10;				/* Next digit			*/
   }					/* And continue until there are */
   while(i);				/* no more digits left		*/

   backstr[j] = '\0';			/* End of the string		*/

   for(l=j-1; m<j; l--)			/* Backstr is backwards (last   */
   {					/* digit first.)  Now we flip   */
      s[m++] = backstr[l];		/* it around...			*/
   }					/* ... and it's ready !!!	*/

   s[j] = '\0';			/* Put the string end on it	*/
}
