#ifndef _BIOS32_H_
#define _BIOS32_H_

union bios32
{
	struct 
	{
		unsigned long signature;	// "_32_"
		unsigned long entry;		// physical address
		unsigned char revision;		// revision level
		unsigned char length;		// length in paragraphs
		unsigned char checksum;		// all bytes must add up to zero
		unsigned char reserved[4];
	} fields;
	char chars[15];
};

struct B32_Service
{
	bool avaliable;		// is there a bios function avaliable
	union
	{
		unsigned long value;
		char str[3];
	} sig;
	char endstr;
	unsigned long base;
	unsigned long length;
	unsigned long offset;
	union
	{
		unsigned long address;
		void (* func)();
	} entry;
} __attribute__ ((packed));

struct PCI_Info
{
	bool installed;		// is there a pci function avaliable
	union
	{
		unsigned long value;
		char str[3];
	} sig;
	char endstr;
	unsigned short verhigh;
	unsigned short verlow;
	unsigned long buses;
} __attribute__ ((packed));

class PmBios
{
	public:
		PmBios();
		bool present();
		bool pci(PCI_Info &struc);

	private:
		bool B32Caller(unsigned long sig, B32_Service &B32service);
		void checkPCI();

		union bios32 dirTable;		//Pointer to the service directory header
		bool avaliable;				//bios32 present?
		B32_Service services[1];	//services supported by the bios
		PCI_Info pcibios;
};

extern PmBios bios32service;

#endif
