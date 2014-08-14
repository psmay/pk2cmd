//                            Software License Agreement
//
// Copyright (c) 2005-2009, Microchip Technology Inc. All rights reserved.
// Portions of this code by Jeff Post, j_post <AT> pacbell <DOT> net
//
// You may use, copy, modify and distribute the Software for use with Microchip
// products only.  If you distribute the Software or its derivatives, the
// Software must have this entire copyright and disclaimer notice prominently
// posted in a location where end users will see it (e.g., installation program,
// program headers, About Box, etc.).  To the maximum extent permitted by law,
// this Software is distributed "AS IS" and WITHOUT ANY WARRANTY INCLUDING BUT
// NOT LIMITED TO ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR
// PARTICULAR PURPOSE, or NON-INFRINGEMENT. IN NO EVENT WILL MICROCHIP OR ITS
// LICENSORS BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
// DAMAGES OF ANY KIND ARISING FROM OR RELATED TO THE USE, MODIFICATION OR
// DISTRIBUTION OF THIS SOFTWARE OR ITS DERIVATIVES.
//
//---------------------------------------------------------------------------
#include <iostream>

#include "stdafx.h"
#include "stdlib.h"
#include "cmd_app.h"
#include "string.h"
#include "time.h"
#include    <termios.h>
#include    <sys/ioctl.h>
extern bool verbose;

extern "C"{
	#include "strnatcmp.h"
}

#include "LongText.h"

#define DEVICE_FILE_NAME "PK2DeviceFile.dat"
#define DEVICE_FILE_NAME_LENGTH 17

#define NSIZE(v) (int)(v.size())

Ccmd_app::Ccmd_app(void)
{
	preserveEEPROM = false;
	hexLoaded = false;
	usingLowVoltageErase = false;
	resetOnExit = false;
	Pk2Operation = true;
	pk2UnitIndex = 0;		
	ReturnCode = OPSUCCESS;
}

Ccmd_app::~Ccmd_app(void)
{
}

void Ccmd_app::PK2_CMD_Entry(TextVec& args)
{
	_TCHAR tempString[MAX_PATH] = "";

	processArgvForSpaces(args);
	args = nargs;

	// Check for help display requests
	if (checkHelp1(args))
	{
		return;
	}

	// Load device file
	bool loadDeviceFileFailed = false;
	if (checkDevFilePathOptionB(args, tempString))
	{ // check for explicit path with -B
		if (tempString[TXT_LENGTH(tempString)-1] != '/')
			TXT_PUSH_UNSAFE(tempString, "/" DEVICE_FILE_NAME);
		else
			TXT_PUSH_UNSAFE(tempString, DEVICE_FILE_NAME);
	}
	else if (ReturnCode == INVALID_CMDLINE_ARG)
	{ // -B, but bad path
		return;
	}
	else
	{ // no -B, check PATH
		_tsearchenv_s(DEVICE_FILE_NAME, "PATH", tempString);
		if (TXT_LENGTH(tempString) < DEVICE_FILE_NAME_LENGTH)
		{
			_tcsncpy_s(tempString, DEVICE_FILE_NAME, DEVICE_FILE_NAME_LENGTH);
		}
	}

	if (!PicFuncs.ReadDeviceFile(tempString))
	{
		cout << format(DEVICE_FILE_NAME " device file not found.") << endl;
		ReturnCode = DEVICEFILE_ERROR;
		loadDeviceFileFailed = true;
	}
	else
	{
		char compatMinLevel = DevFileCompatLevelMin;
		if (PicFuncs.DevFile.Info.Compatibility < compatMinLevel)
		{
			cout << format(DEVICE_FILE_NAME " device file is too old.") << endl;
			ReturnCode = DEVICEFILE_ERROR;
			loadDeviceFileFailed = true;
		}
		if (PicFuncs.DevFile.Info.Compatibility > DevFileCompatLevel)
		{
			cout << format(DEVICE_FILE_NAME " device file requires an update of pk2cmd.") << endl;
			ReturnCode = DEVICEFILE_ERROR;
			loadDeviceFileFailed = true;
		}
	}

	// Check for help display requests that need the device file.
	if (checkHelp2(args, loadDeviceFileFailed) || loadDeviceFileFailed)
	{
		return;
	}

	// Check for Pk2Operation
	Pk2Operation = Pk2OperationCheck(args);

	// Look for PICkit 2
	if (Pk2Operation)
	{
		if (!selectUnitArg(args))
			return; // just listing units

		if (!findPICkit2(pk2UnitIndex))
		{
			if (ReturnCode == WRONG_OS)
				bootloadArg(args); // see if -d found
			return;
		}
	}

	// execute commands
	processArgs(args);

	if (Pk2Operation)
	{
		int status = PicFuncs.ReadPkStatus();
		if ((STATUS_VDD_ERROR & status) > 0)
		{
			cout << "VDD Error detected.  Check target for proper connectivity." << endl;
			ReturnCode = VOLTAGE_ERROR;
		}
		else if ((STATUS_VPP_ERROR & status) > 0)
		{
			cout << "VPP Error detected.  Check target for proper connectivity." << endl;
			ReturnCode = VOLTAGE_ERROR;
		}
	}

}

void Ccmd_app::ResetAtExit(void)
{
	if (resetOnExit)
	{
		cout << "Resetting PICkit 2..." << endl;
		PicFuncs.ResetPICkit2(); // must re-enumerate with new UnitID in serial string
	}
}

bool Ccmd_app::Pk2OperationCheck(TextVec& args)
{
	int i;
	bool ret = false;

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];
		if (checkSwitch(*parg))
		{
			switch((*parg)[1])
			{ // these options require PICkit 2 communications.
				case 'A':
				case 'a':
				case 'C':
				case 'c':
				case 'D':
				case 'd':
				case 'E':
				case 'e':
				case 'G':
				case 'g':
				case 'I':
				case 'i':
				case 'M':
				case 'm':
				case 'N':
				case 'n':
				case 'R':
				case 'r':
				case 'S':
				case 's':
				case 'T':
				case 't':
				case 'U':
				case 'u':
				case 'V':
				case 'v':
				case 'W':
				case 'w':
				case 'X':
				case 'x':
				case 'Y':
				case 'y':
				case 'Z':
				case 'z':
					ret = true;
					break;

				case 'P':
				case 'p':
					//if (((*parg)[2]) == 0)
					//	ret = true; // auto detect
					//if (((((*parg)[2]) == 'F') || (((*parg)[2]) == 'f')) && (((*parg)[3]) != 0))
					//	ret = true; // family detect
					ret = true; // always true so Vdd gets shut off, MCLR is released
					break;

				default:
					break;
			}

			if (ret)
				return true;
		}
	}
	return false;
}

void Ccmd_app::processArgvForSpaces(TextVec& args)
{
	int	i, j;

	// Blank nargv.

	//nargc = 0;

	//for (i=0; i < K_MAX_ARGS; i++)
	//	nargv[i] = NULL;

	nargs.clear();

	for (i=0, j=0; i < NSIZE(args); i++, j++)
	{
		_TCHAR** parg = &args[i];

		nargs.push_back( (char *) malloc(MAX_PATH) );
		strcpy(nargs[j], *parg);

		if (checkSwitch(*parg))
		{
			if ((i < (NSIZE(args) - 1)) && (strlen(*parg) == 2)) // only append next string if first is just option
			{
				if (!checkSwitch(args[i + 1]))
				{
					++i;
					strcat(nargs[j], args[i]);
				}
			}
		}
	}

}

void Ccmd_app::processArgs(TextVec& args)
{
	int i;
	_TCHAR tempString[MAX_PATH] = "";

	// make sure VDD is off
	if (Pk2Operation)
	{
		PicFuncs.VddOff();
	}

	if (bootloadArg(args)) // ignore all other commands if -d found
		return;

	if (unitIDArg(args))	// ignore other commands if -n found
		return;

	_TCHAR** parg = NULL;

	// look for part name first
	for (i = 0; i < NSIZE(args); i++)
	{
		parg = &args[i];
		if (checkSwitch(*parg))
		{
			if (((*parg)[1] == 'P') || ((*parg)[1] == 'p'))
				break;
		}
	}
	if (i == NSIZE(args))
	{ // no part specified
		cout << "-P is a required option" << endl << endl;
		ReturnCode = INVALID_CMDLINE_ARG;
		return;
	}
	XCOPY28(tempString, XRIGHT(*parg,2));
	*parg = (char *) ""; // blank argument, we've already processed it.
	string2Upper(tempString, MAX_PATH);

	// auto detect?
	if (tempString[0] == 0) 
	{ // no argument, full autodetect
		if (detectAllFamilies(args))
		{ // found a device
			XCOPY28(tempString, PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].PartName);
			cout << format("Auto-Detect: Found part %s.", tempString) << endl << endl;
		}
		else if (ReturnCode == INVALID_CMDLINE_ARG)
		{ // arg error
			return;
		}
		else
		{
			cout << "Auto-Detect: No known part found." << endl << endl;
			ReturnCode = AUTODETECT_FAILED;
			return;
		}
	}
	else if (((tempString[0] == 'f') || (tempString[0] == 'F')) && (tempString[1] == 0))
	{ // print family ID numbers
		printFamilies();
		return;
	}
	else  if (((tempString[0] == 'f') || (tempString[0] == 'F')) && (tempString[1] != 0))
	{ // auto detect family
		if (detectSpecificFamily(XRIGHT(tempString,1), args))
		{ // found a device
			XCOPY28(tempString, PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].PartName);
			cout << format("Auto-Detect found part %s.", tempString) << endl << endl;
		}
		else if (ReturnCode == INVALID_CMDLINE_ARG)
		{ // arg error
			return;
		}
		else
		{ // detect failed
			ReturnCode = AUTODETECT_FAILED;
			return;
		}
	}

	// look for the device in the device file - still need to do this on autodetect to properly set up buffers.
	if(!PicFuncs.FindDevice(tempString))
	{
		cout << format("Could not find device %s.", tempString) << endl << endl;
		ReturnCode = INVALID_CMDLINE_ARG;
		return;
	}

	if (!priority1Args(args, false))
		return;
	if (!checkArgsForBlankCheck(args))
	{
		cout << "-C Blank Check must be done independent of other programming commands." << endl;
		ReturnCode = INVALID_CMDLINE_ARG;
		return;
	}
	if (!priority2Args(args))
		return;

	if (!priority3Args(args))
	return;

	if (!priority4Args(args))
	return;

	if (!delayArg(args))
	return;

	// unrecognized commands ignored.
}

bool Ccmd_app::detectAllFamilies(TextVec& args)
{
	// on auto detect, must run these args first
	if (!priority1Args(args, true))
		return false;
    for (int searchIndex = 0; searchIndex < PicFuncs.DevFile.Info.NumberFamilies ; searchIndex++)
    {
		for (int searchPriority = 0; searchPriority < PicFuncs.DevFile.Info.NumberFamilies; searchPriority++)
		{
			if ((PicFuncs.DevFile.Families[searchPriority].PartDetect) && (PicFuncs.DevFile.Families[searchPriority].SearchPriority == searchIndex))
			{
				if (PicFuncs.SearchDevice(searchPriority))   
				{
					return true;
				}
			}
		}
    }
    return false; // no supported part found in any family
}

void Ccmd_app::printFamilies(void)
{
	cout << endl << "Auto-Detectable Part Families:" << endl << endl;
	cout << "     ID#  Family" << endl;
	int familyID = 0;
    for (int index = 0; index < PicFuncs.DevFile.Info.NumberFamilies ; index++)
    {
		for (int order = 0; order < PicFuncs.DevFile.Info.NumberFamilies; order++)
		{
			if ((PicFuncs.DevFile.Families[order].FamilyType == index) && PicFuncs.DevFile.Families[order].PartDetect)
			{
				cout << format("     %2i   %s", familyID++, PicFuncs.DevFile.Families[order].FamilyName) << endl;
			}
		}
    }
}

bool Ccmd_app::detectSpecificFamily(_TCHAR* idString, TextVec& args)
{
	int familyID = 0;

	if (!getValue((unsigned int*)&familyID, idString))
	{
		cout << "-PF Illegal family ID value." << endl;
		ReturnCode = INVALID_CMDLINE_ARG;
		return false;
	}
	// on auto detect, must run these args first
	if (!priority1Args(args, true))
		return false;

	int idNumber = 0;
    for (int index = 0; index < PicFuncs.DevFile.Info.NumberFamilies ; index++)
    {
		for (int order = 0; order < PicFuncs.DevFile.Info.NumberFamilies; order++)
		{
			if ((PicFuncs.DevFile.Families[order].FamilyType == index) && PicFuncs.DevFile.Families[order].PartDetect)
			{
				if (idNumber++ == familyID)
				{
					if (PicFuncs.SearchDevice(order))   
					{
						return true;
					}
					cout << format("Auto-Detect: No known %s part found.", PicFuncs.DevFile.Families[order].FamilyName) << endl << endl;
					return false;
				}
			}
		}
    }
	cout << "-PF Illegal family ID value." << endl;
	ReturnCode = INVALID_CMDLINE_ARG;
	return false;
}

bool Ccmd_app::bootloadArg(TextVec& args)
{
	int i, j;
	_TCHAR tempString[MAX_PATH] = "";
	bool ret;

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		// -D download OS
		if ((checkSwitch(*parg)) && (((*parg)[1] == 'D') || ((*parg)[1] == 'd')))
		{
			PicFuncs.ClosePICkit2Device();
			if ((pk2UnitIndex > 0) || (PicFuncs.DetectPICkit2Device(1, false)))
			{
				cout << endl << "To update the PICkit 2 OS, it must be the only unit connected." << endl;
				ReturnCode = OPFAILURE;
				return true;
			}

			PicFuncs.ClosePICkit2Device();
			PicFuncs.DetectPICkit2Device(0, true);

			XRIGHTCOPY(tempString,*parg,2);
			*parg = (char *) "";
			j = 1;
			while (((i+j) < NSIZE(args)) && (!checkSwitch(args[i+j])))
			{ // check for path with space(s) in it
				TXT_PUSH_UNSAFE(tempString, " ");
				TXT_PUSH_UNSAFE(tempString, args[i+j]);
				args[i + j++] = (char *) "";
			}
			ret = Pk2BootLoadFuncs.ReadHexAndDownload(tempString, &PicFuncs, pk2UnitIndex);
			if (!ret)
			{
				cout << "Error opening hex file." << endl;
				ReturnCode = OPFAILURE;
				return true; // download command found
			}
			ret = Pk2BootLoadFuncs.ReadHexAndVerify(tempString, &PicFuncs);
			if (!ret)
			{
				cout << "Error validating OS download." << endl;
				ReturnCode = OPFAILURE;
				return true; // download command found
			}
			ret = PicFuncs.BL_WriteFWLoadedKey();
			if (!ret)
			{
				cout << "Error with OS download." << endl;
				ReturnCode = OPFAILURE;
				return true; // download command found
			}
			cout << "Resetting PICkit 2..." << endl;
			PicFuncs.BL_Reset();
			Sleep(5000);
			if (!PicFuncs.DetectPICkit2Device(pk2UnitIndex, true))
			{
				cout << "PICkit 2 failed to reset." << endl;
				ReturnCode = OPFAILURE;
				return true; // download command found
			}
			cout << "OS Update Successful." << endl;
			return true;
		}
	}
	return false; // no bootload command
}

bool Ccmd_app::unitIDArg(TextVec& args)
{
	int i, j;
	_TCHAR writeString[MAX_PATH] = "";
	_TCHAR readString[MAX_PATH] = "";
	bool ret;

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		// -N set Unit ID
		if ((checkSwitch(*parg)) && (((*parg)[1] == 'N') || ((*parg)[1] == 'n')))
		{
			XRIGHTCOPY(writeString,*parg,2);
			*parg = (char *) "";
			j = 1;
			while (((i+j) < NSIZE(args)) && (!checkSwitch(args[i+j])))
			{ // check for name with space(s) in it
				TXT_PUSH_UNSAFE(writeString, " ");
				TXT_PUSH_UNSAFE(writeString, args[i+j]);
				args[i + j++] = (char *) "";
			}
			ret = PicFuncs.UnitIDWrite(writeString);
			if (!ret)
			{
				cout << "Error writing Unit ID." << endl;
				ReturnCode = OPFAILURE;
				return true; // unit id command found
			}
			ret = PicFuncs.UnitIDRead(readString);
			if ((TXT_LENGTH(writeString) == 0) && !ret)
			{
				cout << "Unit ID successfully cleared..." << endl;
				resetOnExit = true;
				return true;
			}
			for (j = 0; j < 14; j++)
			{
				if ((writeString[j] != readString[j]) || !ret)
				{
					cout << "Error verifying Unit ID." << endl;
					ReturnCode = OPFAILURE;
					return true; // unit id command found
				}
				if (writeString[j] == 0)
					break;
			}
			cout << "Unit ID successfully assigned..." << endl;
			resetOnExit = true;
			return true;
		}
	}
	return false; // no unit id command
}

bool Ccmd_app::selectUnitArg(TextVec& args)
{
	int i, j, k, len;
	bool listFWVer = false;
	_TCHAR unitIDString[MAX_PATH] = "";
	_TCHAR readString[MAX_PATH] = "";
	_TCHAR *pUnitID = 0;

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		// -S use Unit ID
		if ((checkSwitch(*parg)) && (((*parg)[1] == 'S') || ((*parg)[1] == 's')))
		{
			if ((TXT_LENGTH(*parg) == 3) && ((*parg)[2] == '#'))
				listFWVer = true;

			if ((TXT_LENGTH(*parg) > 2) && !listFWVer)
			{ // find specific unit
				XRIGHTCOPY(unitIDString,*parg,2);
				*parg = (char *) "";
				for (j = 0; j < 8; j++)
				{
					if (PicFuncs.DetectPICkit2Device(j, false))
					{
						//ret = PicFuncs.UnitIDRead(readString);

						pUnitID = PicFuncs.GetUnitID();
						XRIGHTCOPY(readString, pUnitID, 0);
						if (TXT_COMPARE(readString, "-", 1) != 0) // UnitID not blank
						{
							k = TXT_COMPARE(unitIDString, readString, TXT_LENGTH(unitIDString));
							if (k == 0)
							{
								pk2UnitIndex = j;
								PicFuncs.ClosePICkit2Device();	// we'll re-open it when we check OS version, etc.
								return true;
							}
						}
						PicFuncs.ClosePICkit2Device();
					}
					else
					{
						if (j == 0)
							cout << endl << "No PICkit 2 Units Found..." << endl;
						else
							cout << endl << format("PICkit 2 with Unit ID '%s' not found.", unitIDString) << endl;
						break;
					}
				}				
				return false;
			}
			else
			{ // list all units, max 8
				for (j = 0; j < 8; j++)
				{
					if (PicFuncs.DetectPICkit2Device(j, listFWVer))
					{
						if (j == 0)
						{
							if (listFWVer)
								cout << endl << "Unit #     Unit ID          OS Firmware" << endl;
							else
								cout << endl << "Unit #     Unit ID" << endl;
						}

						//if ((PicFuncs.FirmwareVersion.major < 2) || (PicFuncs.FirmwareVersion.major == 'v'))
						//	ret = 0;
						//else
						//{
						//	ret = PicFuncs.UnitIDRead(readString);
						//}
							
						pUnitID = PicFuncs.GetUnitID();
						XRIGHTCOPY(readString, pUnitID, 0);

						//if (ret)
						//{
						//	len = cout << format("%d          %s", j, readString);
						//}
						//else
						//{
						//	len = cout << format("%d          -", j);
						//}
						if (TXT_COMPARE(readString, "PIC18F2550", 10) == 0)
						{
							string stmp;
							if (listFWVer)
								stmp = format("%d          -", j);
							else
								stmp = format("%d          <bootloader>", j);

							cout << stmp;
							len = NSIZE(stmp);
						}
						else
						{
							string stmp;
							stmp = format("%d          %s", j, readString);
							cout << stmp;
							len = NSIZE(stmp);
						}

						while (len < 28)
						{
							cout << " ";
							len++;
						}
						
						if (listFWVer)
						{
							if (PicFuncs.FirmwareVersion.major == 'v')
								cout << "<bootloader>";
							else
								cout << format("%d.%02d.%02d",
									PicFuncs.FirmwareVersion.major,
									PicFuncs.FirmwareVersion.minor,
									PicFuncs.FirmwareVersion.dot); 
						}

						cout << endl;
						PicFuncs.ClosePICkit2Device();
					}
					else
					{
						if (j == 0)
							cout << endl << "No PICkit 2 Units Found..." << endl;
						break;
					}
				}
				return false;
			}
		}
	}
	return true;
}

int Ccmd_app::getPk2UnitIndex(void)
{
	return pk2UnitIndex;
}

void Ccmd_app::string2Upper(_TCHAR* lcstring, int maxLength)
{
	for (int i = 0; i < maxLength; i++)
	{
		if (*(lcstring + i) == 0)
			break;
		else
			*(lcstring + i) = TCH_UPPER(*(lcstring + i));
	}
}

bool Ccmd_app::priority1Args(TextVec& args, bool preserveArgs)
{	// returns false if any command has an error.

	// priority 1 args are -A, -F, -J, -Q, -V, -W, -X, -Z
	// These can be processed before any programming communications
	int i, j;
	unsigned int tempi;
	float tempf;
	bool ret = true;

	_TCHAR tempString[MAX_PATH] = "";

	// Get default Vdd & Vpp
	if (preserveArgs)
	{
		PicFuncs.SetVddSetPoint(3.0); // 3 Volts always used for part detect.
	}
	else
	{
		PicFuncs.GetDefaultVdd();
		PicFuncs.GetDefaultVpp();
	}

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		if (checkSwitch(*parg))
		{
			switch((*parg)[1])
			{
				case 'A':
				case 'a':
					// Set VDD voltage
					if (!preserveArgs) // skip during auto-detect
					{
						tempf = (float)TXT_TO_DOUBLE(XRIGHT(*parg,2));
						if (tempf > PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].VddMax)
						{
							cout << format("-A Vdd setpoint exceeds maximum for this device of %.1fV",
									PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].VddMax) << endl;
							ret = false;
							ReturnCode = INVALID_CMDLINE_ARG;
						}
						if (tempf < 2.5)
						{
							cout << "-A Vdd setpoint below PICkit 2 minimum of 2.5V" << endl;
							ret = false;
							ReturnCode = INVALID_CMDLINE_ARG;
						}
						PicFuncs.SetVddSetPoint(tempf);
						*parg = (char *)"";
					}
					break;

				case 'F':
				case 'f':
					if (!preserveArgs) // skip if still looking for a part
					{
						// Hex File Selection
						XRIGHTCOPY(tempString,*parg,2);
						*parg = (char *) "";
						j = 1;
						while (((i+j) < NSIZE(args)) && (!checkSwitch(args[i+j])))
						{ // check for path with space(s) in it
							TXT_PUSH_UNSAFE(tempString, " ");
							TXT_PUSH_UNSAFE(tempString, args[i+j]);
							if (!preserveArgs)
								args[i + j] = (char *) "";
							j++;
						}
						// Check for BIN file:
						ret = false; // assume not bin file
						j = TXT_LENGTH(tempString);
						if (j > 3)
						{ // this is kind of brute force, but avoids a lot of string library calls and another tempstring
							if (tempString[j-1] == ' ') // may have space on the end
							{
								tempString[j-1] = 0; // kill the space
								j--;
							}
							if (((tempString[j-3] == 'b') || (tempString[j-3] == 'B'))
								&& ((tempString[j-2] == 'i') || (tempString[j-2] == 'I'))
								&& ((tempString[j-1] == 'n') || (tempString[j-1] == 'N')))
								ret = true; // BIN file
						}
						if (ret && PicFuncs.FamilyIsEEPROM())
						{ // bin file
							cout << "Importing -f file as .BIN" << endl;
							ret = ImportExportFuncs.ImportBINFile(tempString, &PicFuncs);
						}
						else
						{ // hex file
							ret = ImportExportFuncs.ImportHexFile(tempString, &PicFuncs);
						}
						if (ret)
							hexLoaded = true;
						else
							ReturnCode = INVALID_HEXFILE;
					}
					break;

				case 'J':
				case 'j':
					// Display percentage operation completion
					if (((*parg)[2] == 'n') || ((*parg)[2] == 'N'))
						PicFuncs.SetTimerFunctions(true, true); // use newlines
					else
						PicFuncs.SetTimerFunctions(true, false);
					if (!preserveArgs)
						*parg = (char *) "";
					break;

				case 'L':
				case 'l':
					// Set ICSP speed
					if ((*parg)[2] == 0)
					{ // no specified value - illegal
						cout << "-L Invalid value." << endl;
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					else if (getValue(&tempi, XRIGHT(*parg,2)))
					{
						if (tempi > 16)
							tempi = 16;
						PicFuncs.SetProgrammingSpeedDefault((unsigned char)tempi);
					}
					else
					{ // no specified value - illegal
						cout << "-L Invalid value." << endl;
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					if (!preserveArgs)
						*parg = (char *) "";
					break;

				case 'Q':
				case 'q':
					// Disable PE
					PicFuncs.DisablePE33();
					if (!preserveArgs)
						*parg = (char *) "";
					break;

				case 'V':
				case 'v':
					// VPP override
					if (preserveArgs)
					{ // cannot be used with part detect
						cout << "-V Cannot be used with part auto-detect." << endl;
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					else
					{
						tempf = (float)TXT_TO_DOUBLE(XRIGHT(*parg,2));
						PicFuncs.SetVppSetPoint(tempf);
						*parg = (char *) "";
					}
					break;

				case 'W':
				case 'w':
					// External power target
					float vdd, vpp;
					PicFuncs.SetSelfPowered(true);
					PicFuncs.ReadPICkitVoltages(&vdd, &vpp);
					PicFuncs.SetVddSetPoint(vdd);
					if (!preserveArgs)
						*parg = (char *) "";
					break;

				case 'X':
				case 'x':
					// VPP first
					if (preserveArgs)
					{ // cannot be used with part detect
						cout << "-X Cannot be used with part auto-detect." << endl;
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					else
					{
						if (PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].ProgEntryVPPScript == 0)
						{
							cout << "-X This part does not support VPP first program mode" << endl;
							ret = false;
							ReturnCode = INVALID_CMDLINE_ARG;
						}
						PicFuncs.SetVppFirstEnable(true);	
							*parg = (char *) "";
					}
					break;

				case 'Z':
				case 'z':
					for (j = 1; j < NSIZE(args); j++)
					{
						if ((checkSwitch(args[j])) && ((args[j][1] == 'M') || (args[j][1] == 'm')))
						{
							preserveEEPROM = true;
						}
					}
					if (!preserveEEPROM)
					{
						cout << "-Z Preserve EEData must be used in conjunction with the -M program command." << endl;
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					if (!preserveArgs)
						*parg = (char *) "";
					break;

				default:
					break;
			}
			if (!ret)
				break;	// stop on first error.
		}
	}	
	if (PicFuncs.GetSelfPowered() && PicFuncs.GetVppFirstEnable())
	{
		cout << "-W -X VPP first not supported with external power" << endl;
		ret = false;
		ReturnCode = INVALID_CMDLINE_ARG;
	}

	return ret;
}

bool Ccmd_app::checkArgsForBlankCheck(TextVec& args)
{	// returns false if there is an error.

	// Blank Check (-C) cannot be used with -E, -G, -M, -U, -Y
	bool blankCheck = false;
	bool contradication = false;

	for (int i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		if (checkSwitch(*parg))
		{
			switch((*parg)[1])
			{
				case 'C':
				case 'c':
					blankCheck = true;
					break;

				case 'E':
				case 'e':
					contradication = true;
					break;

				case 'G':
				case 'g':
					contradication = true;
					break;

				case 'M':
				case 'm':
					contradication = true;
					break;

				case 'U':
				case 'u':
					contradication = true;
					break;

				case 'Y':
				case 'y':
					contradication = true;
					break;

				default:
					break;
			}
		}
	}	

	return !(blankCheck && contradication);
}

bool Ccmd_app::priority2Args(TextVec& args)
{	// returns false if any command has an error.

	// priority 2 args are -C, -U, -E, -M, -Y, -G
	// In the order they will be processed.
	int i, j;
	bool program, eedata, config, userid;
	bool ret = true;

	_TCHAR tempString[MAX_PATH] = "";

	// get current date & time
	_TCHAR stime[128] = "";
	time_t now;
	struct tm today;
	_tzset();
	time(&now);
    _localtime64_s(&today, &now);
	_tcsftime( stime, 128, "%d-%b-%Y, %H:%M:%S", &today );


	// Prep PICkit 2 (set Vdd, vpp, download scripts)
	PicFuncs.PrepPICkit2();

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		// -C Blank Check
		if ((checkSwitch(*parg)) && (((*parg)[1] == 'C') || ((*parg)[1] == 'c')) && ret)
		{
			if (PicFuncs.FamilyIsKeeloq())
			{
				cout << "BlankCheck not supported for KEELOQ devices." << endl;
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
			else if (PicFuncs.FamilyIsMCP())
			{
				cout << "BlankCheck not supported for MCP devices." << endl;
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
			else if (PicFuncs.ReadDevice(BLANK_CHECK, true, true, true, true))
			{
				cout << "Device is blank" << endl;
			}
			else
			{
				cout << format("%s memory is NOT blank.", PicFuncs.ReadError.memoryType) << endl << endl;
				printMemError();
				ret = false;
			}
		}
	}

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		// -U Overwrite Cal
		if ((checkSwitch(*parg)) && (((*parg)[1] == 'U') || ((*parg)[1] == 'u')) && ret)
		{
			if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].OSSCALSave)
			{
				for (j = 1; j < NSIZE(args); j++)
				{
					if ((checkSwitch(args[j])) && ((args[j][1] == 'M') || (args[j][1] == 'm')))
					{
						ret = getValue(&PicFuncs.DeviceBuffers->OSCCAL, XRIGHT(*parg,2));
						if (ret)
						{
							PicFuncs.OverwriteOSCCAL = true;
						}
						else
						{
							cout << "-U Error parsing value." << endl;
							ReturnCode = INVALID_CMDLINE_ARG;
						}
					}
				}
				if (!PicFuncs.OverwriteOSCCAL)
				{
					cout << "-U Overwrite OSCCAL must be used in conjunction with the -M program command." << endl;
					ret = false;
					ReturnCode = INVALID_CMDLINE_ARG;
				}
			}
			else
			{
					cout << "-U Overwrite OSCCAL cannot be used with this device." << endl;
					ret = false;
					ReturnCode = INVALID_CMDLINE_ARG;
			}
		}
	}

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		// -E Erase
		if ((checkSwitch(*parg)) && (((*parg)[1] == 'E') || ((*parg)[1] == 'e')) && ret)
		{
			if (PicFuncs.FamilyIsKeeloq())
			{
				cout << "Erase not supported for KEELOQ devices." << endl;
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
			else if (PicFuncs.FamilyIsMCP())
			{
				cout << "Erase not supported for MCP devices." << endl;
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
			else if (PicFuncs.FamilyIsEEPROM() 
				&& (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigMasks[PROTOCOL_CFG] != MICROWIRE_BUS)
				&& (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigMasks[PROTOCOL_CFG] != UNIO_BUS))
			{ // Microwire / UNIO have a true "chip erase".  Other devices must write every byte blank.
				cout << "Erasing Device..." << endl;
				if (!PicFuncs.SerialEEPROMErase())
				{
					ret = false;
					ReturnCode = OPFAILURE;
				}
			}
			else
			{
				cout << "Erasing Device..." << endl;
				PicFuncs.EraseDevice(true, !preserveEEPROM, &usingLowVoltageErase);
			}
		}
	}

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		// -M Program 
		if ((checkSwitch(*parg)) && (((*parg)[1] == 'M') || ((*parg)[1] == 'm')) && ret)
		{
			if (hexLoaded)
			{
				bool noProgEntryForVerify = false;
				bool verify = true;
				bool argError = true;
								
				if ((*parg)[2] == 0)
				{ // no specified region - erase then program all
					if (PicFuncs.FamilyIsEEPROM())
					{
						ret = PicFuncs.EepromWrite(WRITE_EE);
						verify = ret;
						argError = ret;
						if (!ret)
						{
							ReturnCode = PGMVFY_ERROR;
						}
						program = true;
						eedata = false;
						userid = false;
						config = false;
					}
					else
					{
						bool rewriteEE = PicFuncs.EraseDevice(true, !preserveEEPROM, &usingLowVoltageErase);
						program = true;
						eedata = (rewriteEE || !preserveEEPROM);
						userid = true;
						config = true;
						if (PicFuncs.FamilyIsPIC32())
						{
							// Program everything.
							noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
						}
						else
						{
							// program all but configs and verify, as configs may contain code protect
							noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, false, usingLowVoltageErase);
						}
						if (!noProgEntryForVerify)
						{ // if it is true, then configs are in program memory
							ret = PicFuncs.ReadDevice(VERIFY_MEM_SHORT, program, eedata, userid, false);
							verify = ret;
							argError = ret;
							if (ret)
							{ // now program configs
								program = false;
								eedata = false;
								userid = false;
								noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
							}
							else
							{
								ReturnCode = PGMVFY_ERROR;
							}
						}
					}
				}
				else
				{
					program = false;
					eedata = false;
					userid = false;
					config = false;
					for (j = 2; j < TXT_LENGTH(*parg); j++)
					{
						switch ((*parg)[j])
						{
							case 'p':
							case 'P':
								program = true;
								if (PicFuncs.FamilyIsEEPROM())
								{
									ret = PicFuncs.EepromWrite(WRITE_EE);
									argError = ret;
									verify = ret;
								}
								else
								{
									noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
								}
								break;

							case 'e':
							case 'E':
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].EEMem > 0)
								{
									if (preserveEEPROM)
									{
										cout << "Cannot both program and preserve EEData memory." << endl;
										ReturnCode = INVALID_CMDLINE_ARG;
										ret = false;
									}
									else
									{
										eedata = true;
										noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
									}
								}
								else
									ret = false;
								break;

							case 'i':
							case 'I':
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].UserIDWords > 0)
								{
									userid = true;
									noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
								}
								else
									ret = false;
								break;

							case 'c':
							case 'C':
								// check for devices with config in program memory - can't program seperately.
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords > 0)
								{
									    int configLocation = (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigAddr /
																PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].ProgMemHexBytes;
										int configWords = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords;
										if ((configLocation < (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ProgramMem) && (configWords > 0))
										{
											cout << "This device has configuration words in Program Memory." << endl << "They cannot be programmed separately." << endl;
											ReturnCode = INVALID_CMDLINE_ARG;
											ret = false;
										}
										else
										{
											config = true;
											noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
										}
								}
								else
									ret = false;
								break;

							case 'v':
							case 'V':
								{
								XRIGHTCOPY(tempString,*parg,3);
								*parg = (char *) "";
								int k = 1;
								if (((i+k) < NSIZE(args)) && (!checkSwitch(args[i+k])))
								{ // check for space after v
									TXT_PUSH_UNSAFE(tempString, args[i+k]);
									args[i + k++] = (char *) "";
								}
								int vtop = 0;
								int vbot = 0;
								ret = getRange(&vtop, &vbot, tempString);
								if (ret)
								{
									PicFuncs.WriteVector(vtop, vbot);
								}
								}
								break;

							default:
								ret = false;
						}
					}
				}
				if (ret || !argError)
				{
					if (verify)
					{
						if (noProgEntryForVerify)
							ret = PicFuncs.ReadDevice(VERIFY_NOPRG_ENTRY, program, eedata, userid, config);
						else
							ret = PicFuncs.ReadDevice(VERIFY_MEM_SHORT, program, eedata, userid, config);
					}
					cout << "PICkit 2 Program Report" << endl;
					cout << format("%s", stime) << endl;
					cout << format("Device Type: %s", PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].PartName) << endl << endl;
					if (ret)
					{
						cout << "Program Succeeded." << endl;
					}
					else
					{
						cout << format("%s Memory Errors", PicFuncs.ReadError.memoryType) << endl << endl;
						printMemError();
						ReturnCode = PGMVFY_ERROR;
					}
				}
				else
				{
					cout << "Invalid Memory region entered for program" << endl;
					ReturnCode = INVALID_CMDLINE_ARG;
				}
			}
			else
			{
				cout << "No Image loaded." << endl << "Please load a hex file before programming or verifying." << endl;
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
		}
	}

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		// -Y Verify
		if ((checkSwitch(*parg)) && (((*parg)[1] == 'Y') || ((*parg)[1] == 'y')) && ret)
		{
			if (hexLoaded)
			{

				if (PicFuncs.FamilyIsKeeloq())
				{
					cout << "Verify not supported for KEELOQ devices." << endl;
					ret = false;
				}
				else if ((*parg)[2] == 0)
				{ // no specified region - verify all
					program = true;
					eedata = true;
					userid = true;
					config = true;
				}
				else
				{
					program = false;
					eedata = false;
					userid = false;
					config = false;
					for (j = 2; j < TXT_LENGTH(*parg); j++)
					{
						switch ((*parg)[j])
						{
							case 'p':
							case 'P':
								program = true;
								break;

							case 'e':
							case 'E':
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].EEMem > 0)
									eedata = true;
								else
									ret = false;
								break;

							case 'i':
							case 'I':
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].UserIDWords > 0)
									userid = true;
								else
									ret = false;
								break;

							case 'c':
							case 'C':
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords > 0)
								{
									int configLocation = (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigAddr /
															PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].ProgMemHexBytes;
									int configWords = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords;
									if ((configLocation < (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ProgramMem) && (configWords > 0))
									{
										cout << "This device has configuration words in Program Memory." << endl;
										ReturnCode = INVALID_CMDLINE_ARG;
										ret = false;
									}
									else
										config = true;
								}	
								else
									ret = false;
								break;

							default:
								ret = false;
						}
					}
				}
				if (ret)
				{
					ret = PicFuncs.ReadDevice(VERIFY_MEM, program, eedata, userid, config);
					cout << "PICkit 2 Verify Report" << endl;
					cout << format("%s", stime) << endl;
					cout << format("Device Type: %s", PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].PartName) << endl << endl;
					if (ret)
					{
						cout << "Verify Succeeded." << endl;
					}
					else
					{
						cout << format("%s Memory Errors", PicFuncs.ReadError.memoryType) << endl << endl;
						printMemError();
						ReturnCode = PGMVFY_ERROR;
					}
				}
				else
				{
					cout << "Invalid Memory region entered for verify" << endl;
					ReturnCode = INVALID_CMDLINE_ARG;
				}
			}
			else
			{
				cout << "No Image loaded." << endl << "Please load a hex file before programming or verifying." << endl;
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
		}
	}

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		// -G Read 
		if ((checkSwitch(*parg)) && (((*parg)[1] == 'G') || ((*parg)[1] == 'g')) && ret)
		{
			int startAddr = 0;
			int stopAddr = 0;

			if ((*parg)[2] == 0)
			{ // no specified type - illegal
				ret = false;
			}
			else if (PicFuncs.FamilyIsKeeloq())
			{
				cout << "Read not supported for KEELOQ devices." << endl;
				ret = false;
			}
			else
			{
				switch ((*parg)[2])
				{
					case 'f':
					case 'F':
							if (PicFuncs.ReadDevice(READ_MEM, true, true, true, true))
							{
								XRIGHTCOPY(tempString,*parg,3);
								*parg = (char *) "";
								j = 1;
								while (((i+j) < NSIZE(args)) && (!checkSwitch(args[i+j])))
								{ // check for path with space(s) in it
									if (TXT_LENGTH(tempString) != 0) // don't add space if it's between "F" and start of filename
										TXT_PUSH_UNSAFE(tempString, " ");
									TXT_PUSH_UNSAFE(tempString, args[i+j]);
									args[i + j++] = (char *) "";
								}
								// Check for BIN file:
								ret = false; // assume not bin file
								j = TXT_LENGTH(tempString);
								if (j > 3)
								{ // this is kind of brute force, but avoids a lot of string library calls and another tempstring
									if (tempString[j-1] == ' ') // may have space on the end
									{
										tempString[j-1] = 0; // kill the space
										j--;
									}
									if (((tempString[j-3] == 'b') || (tempString[j-3] == 'B'))
										&& ((tempString[j-2] == 'i') || (tempString[j-2] == 'I'))
										&& ((tempString[j-1] == 'n') || (tempString[j-1] == 'N')))
										ret = true; // BIN file
								}
								if (ret && PicFuncs.FamilyIsEEPROM())
								{ // BIN file
									cout << "Exporting -gf file as .BIN" << endl;
									ret = ImportExportFuncs.ExportBINFile(tempString, &PicFuncs);
								}
								else
								{ // hex file
									ret = ImportExportFuncs.ExportHexFile(tempString, &PicFuncs);
								}
								if (ret)
								{
									cout << "Read successfully." << endl;
									hexLoaded = true;
								}
								else
									ReturnCode = FILE_OPEN_ERROR;
							}
							else
							{
								cout << endl << "Read Error" << endl;
								ReturnCode = OPFAILURE;
							}
						break;

					case 'p':
					case 'P':
						// Read Program mem range to screen
						XRIGHTCOPY(tempString,*parg,3);
						*parg = (char *) "";
						j = 1;
						if (((i+j) < NSIZE(args)) && (!checkSwitch(args[i+j])))
						{ // check for space after p
							TXT_PUSH_UNSAFE(tempString, args[i+j]);
							args[i + j++] = (char *) "";
						}
						ret = getRange(&startAddr, &stopAddr, tempString);
						if (ret)
						{
							if (PicFuncs.ReadDevice(READ_MEM, true, false, false, false))
							{
								cout << "Read successfully." << endl;
								printProgramRange(startAddr, stopAddr);
							}
							else
							{
								cout << endl << "Read Error" << endl;
								ReturnCode = OPFAILURE;
							}
						}
						break;

					case 'e':
					case 'E':
						if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].EEMem > 0)
						{
							// Read EE mem range to screen
							XRIGHTCOPY(tempString,*parg,3);
							*parg = (char *) "";
							j = 1;
							if (((i+j) < NSIZE(args)) && (!checkSwitch(args[i+j])))
							{ // check for space after p
								TXT_PUSH_UNSAFE(tempString, args[i+j]);
								args[i + j++] = (char *) "";
							}
							ret = getRange(&startAddr, &stopAddr, tempString);
							if (ret)
							{
								if (PicFuncs.ReadDevice(READ_MEM, false, true, false, false))
								{
									cout << "Read successfully." << endl;
									printEEDataRange(startAddr, stopAddr);
								}
								else
								{
									cout << endl << "Read Error" << endl;
									ReturnCode = OPFAILURE;
								}
							}
						}
						else
							ret = false;
						break;

					case 'i':
					case 'I':
						if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].UserIDWords > 0)
						{
							// Read User IDs to screen
							if (PicFuncs.ReadDevice(READ_MEM, false, false, true, false))
								{
									cout << "Read successfully." << endl;
									printUserIDs();
								}
								else
								{
									cout << endl << "Read Error" << endl;
									ReturnCode = OPFAILURE;
								}
						}
						else
							ret = false;
						break;

					case 'c':
					case 'C':
						if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords > 0)
						{

						    int configLocation = (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigAddr /
													PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].ProgMemHexBytes;
							int configWords = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords;
							if ((configLocation < (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ProgramMem) && (configWords > 0))
							{
								cout << "This device has configuration words in Program Memory." << endl;
								ReturnCode = INVALID_CMDLINE_ARG;
								ret = false;
							}
							else
							{
								// Read Configuration to screen
								if (PicFuncs.ReadDevice(READ_MEM, false, false, false, true))
									{
										cout << "Read successfully." << endl;
										printConfiguration();
									}
									else
									{
										cout << endl << "Read Error" << endl;
										ReturnCode = OPFAILURE;
									}
							}
						}
						else
							ret = false;
						break;

					case 'v':
					case 'V':
						{
						unsigned int vector = PicFuncs.ReadVector();
						cout << format("%8X", vector) << endl;
					    break;
						}

					default:
						ret = false;
				}
			}
			if (!ret)
			{
				if (ReturnCode != FILE_OPEN_ERROR)
				{
					cout << "Illegal read parameter entered." << endl;
					ReturnCode = INVALID_CMDLINE_ARG;
				}
			}
		}
	}	

	return ret;
}

bool Ccmd_app::priority3Args(TextVec& args)
{	// returns false if any command has an error.

	// priority 3 args are -I, -K
	// These can be processed after any programming communications
	int i, j;
	bool ret = true;

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		if (checkSwitch(*parg))
		{
			switch((*parg)[1])
			{
				case 'I':
				case 'i':
					// Display Device ID
					if (PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].PartDetect)
					{
						int deviceID = PicFuncs.ReadDeviceID();
						cout << format("Device ID = %04X", deviceID) << endl;
						cout << format("Revision  = %04X", PicFuncs.GetDeviceRevision()) << endl;
						// Display the device name matching the ID, not necessary the active device.
						if (deviceID == 0)
						{
							cout << "Device Name = <no device>" << endl;
						}
						else
						{
							for (j = 1; j < PicFuncs.DevFile.Info.NumberParts; j++)
							{
								if ((PicFuncs.DevFile.PartsList[j].Family == PicFuncs.ActiveFamily) && (deviceID == (int)PicFuncs.DevFile.PartsList[j].DeviceID) && PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].PartDetect)
								{
									if (j == PicFuncs.ActivePart)
									{
										cout << format("Device Name = %s", PicFuncs.DevFile.PartsList[j].PartName) << endl;
									}
									else
									{
										cout << format("Device Name = %s   !WARNING! -P device mismatch", PicFuncs.DevFile.PartsList[j].PartName) << endl;
									}
									break;
								}
							}
							if (j == PicFuncs.DevFile.Info.NumberParts)
							{ // no matching device found.
								cout << "Device Name = <unknown device>" << endl;
							}
						}
					}
					else
					{
						cout << "This device does not have a Device ID." << endl;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					break;

				case 'K':
				case 'k':
					// Calculate Checksum
					if (hexLoaded)
					{
						cout << format("Checksum = %04X", PicFuncs.ComputeChecksum()) << endl;
					}
					else
					{
						cout << "The checksum can only be calculated when a hex file is loaded or written." << endl;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					break;

				default:
					break;
			}
			if (!ret)
				break;	// stop on first error.
		}
	}	
	return ret;
}

bool Ccmd_app::priority4Args(TextVec& args)
{	// returns false if any command has an error.

	// priority 4 args are -R, -T
	// These can be processed after any programming communications
	int i;
	bool ret = true;

	if (Pk2Operation)
	{
		PicFuncs.SetMCLR(true); // ensure /MCLR asserted.
		if (!PicFuncs.GetSelfPowered())
			PicFuncs.VddOff();      // ensure VDD off if no -T
	}

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		if (checkSwitch(*parg))
		{
			switch((*parg)[1])
			{
				case 'R':
				case 'r':
					// Release /MCLR
					PicFuncs.SetMCLR(false);
					break;

				case 'T':
				case 't':
					// Power Target
					if (PicFuncs.GetSelfPowered())
					{
						cout << "-W -T Cannot power an externally powered target." << endl;
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					else
						PicFuncs.VddOn();
					break;

				default:
					break;
			}
			if (!ret)
				break;	// stop on first error.
		}
	}	
	return ret;
}

bool Ccmd_app::delayArg(TextVec& args)
{	// returns false if command has an error.

	// delay arg is -H
	// This must be processed last
	int i;
	unsigned int seconds;
	bool ret = true;
	struct termios	tios;

	for (i = 1; i < NSIZE(args); i++)
	{
		_TCHAR** parg = &args[i];

		if (checkSwitch(*parg))
		{
			switch((*parg)[1])
			{
				case 'H':
				case 'h':
					// Delay before exit
					if ((*parg)[2] == 0)
					{ // no specified value - illegal
						cout << "-H Invalid value." << endl;
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					else
					{
						if (((*parg)[2] == 'K') || ((*parg)[2] == 'k'))
						{
							cout << endl << "Press any key to exit." << endl;
							tcgetattr(0, &tios);
							tios.c_lflag &= (~(ICANON | ECHO));
							tcsetattr(0, TCSANOW, &tios);
							getc(stdin);
							tios.c_lflag |= (ICANON | ECHO);
							tcsetattr(0, TCSANOW, &tios);
						}
						else if (getValue(&seconds, XRIGHT(*parg,2)))
						{
							if (seconds == 0)
							{ // bad value
								cout << "-H Invalid value." << endl;
								ret = false;
								ReturnCode = INVALID_CMDLINE_ARG;
							}
							else
							{
								cout << endl << format("Delaying %d seconds before exit.", seconds) << endl;
								PicFuncs.DelaySeconds(seconds);
							}
						}
						else 
						{ // bad value
							cout << "-H Invalid value." << endl;
							ret = false;
							ReturnCode = INVALID_CMDLINE_ARG;
						}
					}
					break;

				default:
					break;
			}
			if (!ret)
				break;	// stop on first error.
		}
	}	
	return ret;
}

void Ccmd_app::printProgramRange(int startAddr, int stopAddr)
{
	int addrInc = PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].AddressIncrement;
	int startWord = startAddr / addrInc;
	int stopWord = stopAddr / addrInc;
	int col;

	if (stopWord < startWord)
		stopWord = startWord;

	if (stopWord >= (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ProgramMem)
		stopWord = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ProgramMem - 1;

	cout << endl << "Program Memory";

	do
	{ // address loop
		cout << endl << format("%08X ", startWord * addrInc);

		col = 0;
		if (PicFuncs.FamilyIsEEPROM())
		{
			if (PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].BlankValue > 0xFF)
			{
				do
				{ // data columns loop
					cout << format("%04X  ", PicFuncs.DeviceBuffers->ProgramMemory[startWord++]);
					col++;
				} while ((startWord <= stopWord) && (col < 8));
			}
			else
			{
				do
				{ // data columns loop
					cout << format("%02X  ", PicFuncs.DeviceBuffers->ProgramMemory[startWord++]);
					col++;
				} while ((startWord <= stopWord) && (col < 16));
			}
		}
		else
		{
			do
			{ // data columns loop
				cout << format("%06X  ", PicFuncs.DeviceBuffers->ProgramMemory[startWord++]);
				col++;
			} while ((startWord <= stopWord) && (col < 8));
		}

	} while (startWord <= stopWord);
	cout << endl;
}

void Ccmd_app::printEEDataRange(int startAddr, int stopAddr)
{
	int addrInc = PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].EEMemAddressIncrement;
	int startWord = startAddr / addrInc;
	int stopWord = stopAddr / addrInc;
	int col;

	if (stopWord < startWord)
		stopWord = startWord;

	if (stopWord >= PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].EEMem)
		stopWord = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].EEMem -1;

	cout << endl << "EEData Memory";

	do
	{ // address loop
		cout << endl << format("%04X ", startWord * addrInc);

		col = 0;
		do
		{ // data columns loop
			if (addrInc > 1)
				cout << format("%04X  ", PicFuncs.DeviceBuffers->EEPromMemory[startWord++]);
			else
				cout << format("%02X  ", PicFuncs.DeviceBuffers->EEPromMemory[startWord++]);
			col++;
		} while ((startWord <= stopWord) && (col < 8));

	} while (startWord <= stopWord);
	cout << endl;
}

void Ccmd_app::printUserIDs(void)
{
	int startWord = 0;
	int stopWord = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].UserIDWords -1;
	int col;

	cout << endl << "ID Memory" << endl;

	do
	{ // address loop
		cout << endl;
		col = 0;
		do
		{ // data columns loop
			if (PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].BlankValue > 0xFFFF)
				cout << format("%06X  ", PicFuncs.DeviceBuffers->UserIDs[startWord++]);
			else
				cout << format("%04X  ", PicFuncs.DeviceBuffers->UserIDs[startWord++]);
			col++;
		} while ((startWord <= stopWord) && (col < 8));

	} while (startWord <= stopWord);
	cout << endl;
}

void Ccmd_app::printConfiguration(void)
{
	int startWord = 0;
	int stopWord = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords -1;
	int col;

	cout << endl << "Configuration Memory" << endl;

	do
	{ // address loop
		cout << endl;
		col = 0;
		do
		{ // data columns loop
			if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].BandGapMask == 0)
				cout << format("%04X  ", (PicFuncs.DeviceBuffers->ConfigWords[startWord]
								& PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigMasks[startWord]));
			else
				cout << format("%04X  ", (PicFuncs.DeviceBuffers->ConfigWords[startWord]
								& PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigMasks[startWord])
								| PicFuncs.DeviceBuffers->BandGap);
			startWord++;
			col++;
		} while ((startWord <= stopWord) && (col < 8));

	} while (startWord <= stopWord);
	cout << endl;
}

bool Ccmd_app::getRange(int* start, int* stop, _TCHAR* str_range)
{
	int i, j;
	_TCHAR temps[8] = "";

	if (*str_range == 0)
	{ // no range, return (use defaults)
		return true;
	}

	// get start address
	for (i = 0; i < 9; i++)
	{
		if (*(str_range + i) == '-')
		{
			temps[i] = 0;
			break;
		}
		else if (*(str_range + i) == 0)
		 // unexpected end of string
			return false;
		else
			temps[i] = *(str_range + i);			
	}

	if ((i >= 9) || ( i == 0))
	// more than 8 character address or no address
		return false;

	*start = ImportExportFuncs.ParseHex(temps, i++);

	// get stop address
	for (j = 0; j < 9; j++)
	{
		if (*(str_range + i + j) == 0)
		 // end of string
		{
			temps[j] = 0;
			break;
		}
		else
			temps[j] = *(str_range + i + j);			
	}

	if ((j >= 9) || (j ==0))
	// more than 8 character address or no address
		return false;	

	*stop = ImportExportFuncs.ParseHex(temps, j++);
	if (*start <= *stop)
		return true;
	else
		return false;
}

bool Ccmd_app::getValue(unsigned int* value, _TCHAR* str_value)
{
	int i;
	_TCHAR temps[8] = "";

	if (*str_value == 0)
	{ // no value, return error
		return false;
	}

	// get value
	for (i = 0; i < 9; i++)
	{
		if (*(str_value + i) == 0)
		{
			temps[i] = 0;
			break;
		}
		else
			temps[i] = *(str_value + i);			
	}

	if ((i >= 9) || ( i == 0))
	// more than 8 character value or no value
		return false;

	*value = ImportExportFuncs.ParseHex(temps, i++);

	return true;
}

bool Ccmd_app::checkSwitch(_TCHAR * arg)
{
	return ((arg[0] == '-') || (arg[0] == '/'));
}

bool Ccmd_app::findPICkit2(int unitIndex)
{
	unsigned char dot_min = PicFuncs.FW_DOT_MIN;

	if (PicFuncs.DetectPICkit2Device(unitIndex, true))
	{
		if ((PicFuncs.FirmwareVersion.major >= PicFuncs.FW_MAJ_MIN)
			&& (PicFuncs.FirmwareVersion.minor >= PicFuncs.FW_MNR_MIN)
			&& (PicFuncs.FirmwareVersion.dot >= dot_min))
		{
			return true;
		}
		cout << format("PICkit 2 found with Operating System v%d.%02d.%02d", PicFuncs.FirmwareVersion.major,
									PicFuncs.FirmwareVersion.minor, PicFuncs.FirmwareVersion.dot) << endl;
		cout << format("Use -D to download minimum required OS v%d.%02d.%02d or later", PicFuncs.FW_MAJ_MIN,
									PicFuncs.FW_MNR_MIN, PicFuncs.FW_DOT_MIN) << endl;
		ReturnCode = WRONG_OS;
	}
	else
	{
		cout << "No PICkit 2 found." << endl;
		ReturnCode = NO_PROGRAMMER;
	}
	return false;
}

void Ccmd_app::printMemError(void)
{
	if (!PicFuncs.FamilyIsPIC32() && !PicFuncs.useProgExec33())
	{
		cout << "Address   Good     Bad" << endl;
		cout << format("%06X    %06X   %06X", PicFuncs.ReadError.address, PicFuncs.ReadError.expected, PicFuncs.ReadError.read) << endl;
	}
}

bool Ccmd_app::checkDevFilePathOptionB(TextVec& args, _TCHAR* path_string)
{
	_TCHAR path_temp[MAX_PATH];

	int i;

	_TCHAR** parg = NULL;

	// look for 'B' option. 
	for (i = 0; i < NSIZE(args); i++)
	{
		parg = &args[i];
		if (checkSwitch(*parg))
		{
			if (((*parg)[1] == 'b') || ((*parg)[1] == 'B'))
				break;
		}
	}
	if (i == NSIZE(args))
		return false; // -b not found
	if ((*parg)[2] == 0)
	{
		cout << "-B No path given" << endl;
		ReturnCode = INVALID_CMDLINE_ARG;
		return false;
	}

	// Get path to device file:
	XRIGHTCOPY(path_temp,*parg,2);
	*parg = (char *) "";
	int j = 1;
	while (((i+j) < NSIZE(args)) && (!checkSwitch(args[i+j])))
	{ // check for path with space(s) in it
		TXT_PUSH_UNSAFE(path_temp, " ");
		TXT_PUSH_UNSAFE(path_temp, args[i+j]);
		args[i + j++] = (char *) "";
	}
	i = 0;
	do
	{
		path_string[i] = path_temp[i];
	}while (path_string[i++] != 0);
	return true;
}


bool Ccmd_app::checkHelp1(TextVec& args)
{ // Helps that don't need the device file.
	int i;

	// if no arguments, display main help screen
	if (NSIZE(args) == 1)
	{
		displayHelp();
		return true;
	}

	_TCHAR** parg = NULL;

	// look for '?' in all arguments.  Display help for first found
	for (i = 0; i < NSIZE(args); i++)
	{
		parg = &args[i];

		if (TXT_SEEK_TCHAR(*parg, '?'))
			break;
	}

	if (i == NSIZE(args)) // none found
		return false;

	if (checkSwitch(*parg))
	{
		switch ((*parg)[1])
		{
			case '?':
				if (TXT_LENGTH(*parg) > 2)
				{
					if (((*parg)[2] == 'e') || ((*parg)[2] == 'E'))
					{
						LongText::lx_qe();
					}
					else if (((*parg)[2] == 'l') || ((*parg)[2] == 'L'))
					{
						displayLicense();
					}
					else
					{
						//displayHelp();
						return false; // check later for devfile dependent helps
					}
				}
				else
				{
					displayHelp();
				}
				break;

			case 'a':
			case 'A':
				LongText::lx_a();
				break;
			case 'b':
			case 'B':
				LongText::lx_b();
				break;
			case 'c':
			case 'C':
				LongText::lx_c();
				break;
			case 'd':
			case 'D':
				LongText::lx_d();
				break;
			case 'e':
			case 'E':
				LongText::lx_e();
				break;
			case 'f':
			case 'F':
				LongText::lx_f();
				break;
			case 'g':
			case 'G':
				LongText::lx_g();
				break;
			case 'h':
			case 'H':
				LongText::lx_h();
				break;
			case 'i':
			case 'I':
				LongText::lx_i();
				break;
			case 'j':
			case 'J':
				LongText::lx_j();
				break;
			case 'k':
			case 'K':
				LongText::lx_k();
				break;
			case 'l':
			case 'L':
				LongText::lx_l();
				break;
			case 'm':
			case 'M':
				LongText::lx_m();
				break;
			case 'n':
			case 'N':
				LongText::lx_n();
				break;
			case 'p':
			case 'P':
				LongText::lx_p();
				break;
			case 'q':
			case 'Q':
				LongText::lx_q();
				break;
			case 'r':
			case 'R':
				LongText::lx_r();
				break;
			case 's':
			case 'S':
				LongText::lx_s();
				break;
			case 't':
			case 'T':
				LongText::lx_t();
				break;
			case 'u':
			case 'U':
				LongText::lx_u();
				break;
			case 'v':
			case 'V':
				LongText::lx_v();
				break;
			case 'w':
			case 'W':
				LongText::lx_w();
				break;
			case 'x':
			case 'X':
				LongText::lx_x();
				break;
			case 'y':
			case 'Y':
				LongText::lx_y();
				break;
			case 'z':
			case 'Z':
				LongText::lx_z();
				break;
			default:
				return false; // may be one that needs the device file.
		}
	}
	else
	{ // just display main help
		displayHelp();
	}

	return true;
}


void Ccmd_app::displayHelp(void)
{
	LongText::lx_display_help();
}

bool Ccmd_app::checkHelp2(TextVec& args, bool loadDeviceFileFailed)
{  // helps that need the device file loaded
	int i;

	_TCHAR** parg = NULL;

	// look for '?' in all arguments.  Display help for first found
	for (i = 0; i < NSIZE(args); i++)
	{
		parg = &args[i];

		if (TXT_SEEK_TCHAR(*parg, '?'))
			break;
	}
	
	if (i == NSIZE(args)) // none found
		return false;

	if (checkSwitch(*parg))
	{
		switch ((*parg)[1])
		{
			case '?':
				if (TXT_LENGTH(*parg) > 2)
				{
					if (((*parg)[2] == 'v') || ((*parg)[2] == 'V'))
					{
						cout << endl << format("Executable Version:    %d.%02d.%02d", VERSION_MAJOR, VERSION_MINOR, VERSION_DOT);
						
						if (loadDeviceFileFailed)
							cout << endl << "Device File Version:   not found" << endl;
						else
							cout << endl << format("Device File Version:   %d.%02d.%02d", PicFuncs.DevFile.Info.VersionMajor,
								PicFuncs.DevFile.Info.VersionMinor, PicFuncs.DevFile.Info.VersionDot) << endl;
						// Look for PICkit 2
						selectUnitArg(args);
						if (PicFuncs.DetectPICkit2Device(pk2UnitIndex, true))
						{
							cout << format("OS Firmware Version:   %d.%02d.%02d", PicFuncs.FirmwareVersion.major,
								PicFuncs.FirmwareVersion.minor, PicFuncs.FirmwareVersion.dot) << endl << endl;
						}
						else
						{
							cout << "OS Firmware Version:   PICkit 2 not found" << endl << endl;
						}

					}
					else if (((*parg)[2] == 'p') || ((*parg)[2] == 'P'))
					{
						if (loadDeviceFileFailed)
						{
							cout << "Unable to list parts: Device File Load Failed" << endl << endl;
						}
						else
						{
							_TCHAR searchTerm[MAX_PATH];
							// get search term
							XRIGHTCOPY(searchTerm,*parg,3);
							*parg = (char *) "";
							int j = 1;
							while (((i+j) < NSIZE(args)) && (!checkSwitch(args[i+j])))
							{ // check for term with space(s) in it
								TXT_PUSH_UNSAFE(searchTerm, args[i+j]);
								args[i + j++] = (char *) "";
							}
							displayPartList(args, searchTerm);
						}
					}
					else
					{
						displayHelp();
					}
				}
				break;

			default:
				cout << "Invalid command, or no Help available to for specified command." << endl;

		}
	}

	return true;
}


void Ccmd_app::displayLicense(void)
{
	LongText::lx_display_license();
}



void Ccmd_app::displayPartList(TextVec& args, _TCHAR* argSearch)
{
	_TCHAR *partlist_array[1024];
	int partNum, partIdx;

	string2Upper(argSearch, MAX_PATH);

	cout << endl << format("Device File Version:   %d.%02d.%02d", PicFuncs.DevFile.Info.VersionMajor,
		PicFuncs.DevFile.Info.VersionMinor, PicFuncs.DevFile.Info.VersionDot) << endl;

	// display sorted parts by family, in family display order
	if (argSearch[0] == 0)
		cout << format("Number of devices = %i", PicFuncs.DevFile.Info.NumberParts - 1) << endl << endl; // don't count "unsupported" device
	else
		cout << format("List of devices starting with '%s':", argSearch) << endl << endl;
	cout << "Device Name                  Device Family" << endl;
	cout << "-----------                  -------------" << endl;
	for (int index = 0; index < PicFuncs.DevFile.Info.NumberFamilies ; index++)
	{
		for (int order = 0; order < PicFuncs.DevFile.Info.NumberFamilies; order++)
		{
			if (PicFuncs.DevFile.Families[order].FamilyType == index)
			{
				// get all the parts in this family
				partNum = 0;
				for (partIdx = 1; partIdx < (PicFuncs.DevFile.Info.NumberParts); partIdx++)
				{
					// skip first part, which is "unsupported part"
					if (PicFuncs.DevFile.PartsList[partIdx].Family == order)
						partlist_array[partNum++] = strdup(PicFuncs.DevFile.PartsList[partIdx].PartName);
				}
				// sort them
				qsort(partlist_array, partNum, sizeof(_TCHAR *), strnatcmpWrapper);
				// list them
				if (argSearch[0] == 0)
				{ // list all parts
					for (partIdx = 0; partIdx < partNum; partIdx++)
					{
						cout << format("%-28s %s", partlist_array[partIdx], PicFuncs.DevFile.Families[order].FamilyName) << endl;
					}
				}
				else
				{ // search parts
					int l = TXT_LENGTH(argSearch);
					for (partIdx = 0; partIdx < partNum; partIdx++)
					{
						if (TXT_COMPARE(partlist_array[partIdx], argSearch, l) == 0)
							cout << format("%-28s %s", partlist_array[partIdx], PicFuncs.DevFile.Families[order].FamilyName) << endl;
					}
				}
			}
		}
	}

	LongText::lx_partlist_notice();
}

int Ccmd_app::strnatcmpWrapper(const void *a, const void *b)
{
	return strnatcmp(*(char const **)a, *(char const **)b);
}
