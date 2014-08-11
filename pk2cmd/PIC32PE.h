static const int	K_P32_PROGRAM_FLASH_START_ADDR = 0x1D000000;
static const int	K_P32_BOOT_FLASH_START_ADDR =    0x1FC00000;

static const int	K_PE_LOADER_LEN =	42;
static const int K_PE_LEN =				1231;

static const int K_PIC32_PE_VERSION =	0x0109;

class CPIC32PE
{
	public:
		CPIC32PE(void);
		~CPIC32PE(void);

		static const unsigned int pe_Loader[K_PE_LOADER_LEN];		//PIC32
		static const unsigned int PIC32_PE[K_PE_LEN];		//PIC32
};

