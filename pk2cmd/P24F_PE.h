static const int K_PE24F_LEN =		512;

static const int K_PIC24F_PE_VERSION = 0x0026;
static const int K_PIC24F_PE_ID =	   0x009B;

class CP24F_PE
{
	public:
		CP24F_PE(void);
		~CP24F_PE(void);

		static const unsigned int PIC24F_PE_Code[K_PE24F_LEN];

};

