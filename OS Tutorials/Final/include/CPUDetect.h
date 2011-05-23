#ifndef __CPUDETECT_H__
#define __CPUDETECT_H__


union Ven
{
	struct
	{
		unsigned long ebx;
		unsigned long edx;
		unsigned long ecx;
	} regs;
	char sign[12];
};

struct CPUdata
{
	unsigned char	family;		// 5 = 586, etc
	unsigned char	familyEx;
	unsigned char	model;		// Model of CPU within a family
	unsigned char	modelEx;
	unsigned char	stepping;
	unsigned char	brand;
	unsigned long	flags;		// Functions supported by CPU
	union Ven		vendor;		// CPUID vendor
	unsigned char	name[64];	// ECPUID name
};


class CPU
{
	public:
		CPU();
		CPUdata GetCPU();

	private:
		CPUdata cpuInfo;
		unsigned long MaxEAX;	// contains the maximum value to call CPUID with
};


extern CPU cpuInfo;


#endif


