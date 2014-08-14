#pragma once
#include "PICkitFunctions.h"
#include "ImportExportHex.h"
#include "Pk2BootLoader.h"

static const int K_MAX_ARGS =	32;

// Return Codes
static const int OPSUCCESS =                 0;  // operation completed as expected
static const int VOLTAGE_ERROR =             5;  // Vdd or vpp error detected
static const int OPFAILURE =                 7;  // operation failed
static const int NO_PROGRAMMER =             10; // Unable to find a PICkit 2 programmer
static const int WRONG_OS =                  11; // OS firmware must be upgraded
static const int FILE_OPEN_ERROR =           15; // returned if a file to read to (-gf..) cannot be opened.
static const int DEVICEFILE_ERROR =          24; // the device file was not found or an error occured while loading.
static const int UPDGRADE_ERROR =            28; // returned when OS upgrade fails. 
static const int PGMVFY_ERROR =              34; // returned if program or verify operation fails.
static const int INVALID_CMDLINE_ARG =       36;
static const int INVALID_HEXFILE =           37; // Error reading hex file.
static const int AUTODETECT_FAILED =         39; // Couldn't find a part

class Ccmd_app
{
public:
	Ccmd_app(void);
	~Ccmd_app(void);
	void PK2_CMD_Entry(TextVec& args);
	void ResetAtExit(void);

	// Updated to compat level 6, Aug 2010.
	static const unsigned char DevFileCompatLevel = 6;
	static const unsigned char DevFileCompatLevelMin = 0;

	CPICkitFunctions PicFuncs;
	CImportExportHex ImportExportFuncs;
	Pk2BootLoader Pk2BootLoadFuncs;
	int ReturnCode;

protected:
	void processArgvForSpaces(TextVec& args);
	bool Pk2OperationCheck(TextVec& args);
	bool bootloadArg(TextVec& args);
	bool unitIDArg(TextVec& args);
	bool selectUnitArg(TextVec& args);
	int getPk2UnitIndex(void);
	void string2Upper(_TCHAR* lcstring, int maxLength);
	void processArgs(TextVec& args);
	bool detectAllFamilies(TextVec& args);
	void printFamilies(void);
	bool detectSpecificFamily(_TCHAR* idString, TextVec& args);
	bool priority1Args(TextVec& args, bool preserveArgs);
	bool checkArgsForBlankCheck(TextVec& args);
	bool priority2Args(TextVec& args);
	bool priority3Args(TextVec& args);
	bool priority4Args(TextVec& args);
	bool delayArg(TextVec& args);
	void printProgramRange(int startAddr, int stopAddr);
	void printEEDataRange(int startAddr, int stopAddr);
	void printUserIDs(void);
	void printConfiguration(void);
	bool getRange(int* start, int* stop, _TCHAR* str_range);
	bool getValue(unsigned int* value, _TCHAR* str_value);
	bool checkSwitch(_TCHAR* argv);
	bool findPICkit2(int unitIndex);
	void printMemError(void);
	bool checkDevFilePathOptionB(TextVec& args, _TCHAR* path_string);
	bool checkHelp1(TextVec& args);
	bool checkHelp2(TextVec& args, bool loadDeviceFileFailed);
	void displayHelp(void);
	void displayLicense(void);
	void displayPartList(TextVec& args, _TCHAR* argSearch);
	static int strnatcmpWrapper(const void *a, const void *b);

	bool preserveEEPROM;
	bool hexLoaded;
	bool usingLowVoltageErase;
	bool resetOnExit;
	bool Pk2Operation;		// operation does require connecting to/using PICkit 2
	int	 pk2UnitIndex;
	TextVec nargs;
};
