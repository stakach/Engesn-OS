#ifndef _PIT_H_
#define _PIT_H_

class PIT
{
	public:
		PIT();
		void init(long h, unsigned char channel);
};

void delay(unsigned long milliseconds);

extern PIT pit;

#endif

