#ifndef __ACPI_H__
#define __ACPI_H__


#define RSDP_SIG	"RSD PTR "				// RSDT Pointer signature

union ACPI
{
	struct 
	{
		unsigned long		sig1;
		unsigned long		sig2;
		unsigned char		checksum;
		unsigned char		oem[5];
		unsigned char		revision;		// Revision 0 = ver 1, Revision 2 = ver 2
		unsigned long		phys_rsdt;		// 20 chars (0-19)
		unsigned long		length;
		unsigned long long	phys_xsdt;
		unsigned char		exchecksum;
		unsigned char		Reserved[2];	// 36 chars (0-35)
	} fields;
	char chars[35];
};



class ACPIFunc
{
	public:
		ACPIFunc();
		bool found;

	private:
		union ACPI desTable;
};


extern ACPIFunc acpiService;


#endif

