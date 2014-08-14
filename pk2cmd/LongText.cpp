/*
 * LongText.cpp
 *
 *  Created on: Aug 14, 2014
 *      Author: psmay
 */

#include "LongText.h"

#include <iostream>
using namespace std;

#define DEVICE_FILE_NAME "PK2DeviceFile.dat"

void LongText::lx_qe() {
	cout << "PK2CMD return codes:" << endl
			<< "Value   Code                    Notes" << endl
			<< "-----   ----                    -----" << endl
			<< "0       OPSUCCESS              -Returned if all selected operations complete"
			<< endl << "                                successfully." << endl
			<< "5       VOLTAGE_ERROR          -A Vdd and/or Vpp voltage error was detected."
			<< endl
			<< "                                This could be due to PICkit 2 being "
			<< endl
			<< "                                improperly connected to a part, incorrect"
			<< endl
			<< "                                part selection, or interference from other"
			<< endl
			<< "                                circuitry on the target board."
			<< endl
			<< "7       OPFAILURE              -Returned if an operation fails and a more"
			<< endl
			<< "                                specific error does not exist."
			<< endl
			<< "10      NO_PROGRAMMER          -The PK2CMD executable is unable to find a"
			<< endl
			<< "                                connected PICkit 2 programmer."
			<< endl
			<< "11      WRONG_OS                -Returned if the OS firmware must be updated"
			<< endl
			<< "                                before being used with this version of"
			<< endl << "                                PK2CMD." << endl
			<< "15      FILE_OPEN_ERROR        -Returned if a file specified for reading to"
			<< endl
			<< "                                (-gf...) cannot be opened for writing."
			<< endl
			<< "24      DEVICEFILE_ERROR       -The PK2CMD executable cannot find the device"
			<< endl
			<< "                                file " DEVICE_FILE_NAME " or the device file"
			<< endl << "                                may be corrupted."
			<< endl
			<< "28      UPDGRADE_ERROR         -Returned when an OS firmware upgade (-d...)"
			<< endl << "                                fails." << endl
			<< "34      PGMVFY_ERROR           -Returned if a program or verify operation"
			<< endl << "                                fails." << endl
			<< "36      INVALID_CMDLINE_ARG    -A syntax error in a command line argument"
			<< endl
			<< "                                was detected, an invalid combination of "
			<< endl
			<< "                                operations was entered, an invalid value was"
			<< endl
			<< "                                entered, or a memory region was selected"
			<< endl
			<< "                                that is not supported by the current device."
			<< endl
			<< "37      INVALID_HEXFILE        -Error opening or loading a specified hex"
			<< endl << "                                file (-f...)." << endl
			<< "39      AUTODETECT_FAILED       A part autodetect operation failed to find"
			<< endl << "                                a known part." << endl
			<< endl;
}

void LongText::lx_a() {
	cout
			<< "Specifies the VDD voltage that the device is programmed at.  The value"
			<< endl
			<< "entered must be less than the allowed maximum of the device and 5.0 Volts"
			<< endl
			<< "(whichever is less), and greater than the allowed minimum of the device and"
			<< endl
			<< "2.5 Volts (whichever is greater).  A default voltage for the device will be"
			<< endl << "used if this command is not specified." << endl << endl
			<< "The parameter for this command is the floating point value of the desired"
			<< endl << "VDD voltage." << endl << endl << "Syntax Example -a4.5"
			<< endl;
}

void LongText::lx_b() {
	cout
			<< "Specifies the path to the device file " DEVICE_FILE_NAME ".  By default, the"
			<< endl
			<< "directory from which the executable is searched first, then the PATH"
			<< endl
			<< "environment variable.  This option can be used to explicity specify the"
			<< endl << "path to the device file." << endl << endl
			<< "The parameter for this command is the complete file path to"
			<< endl << DEVICE_FILE_NAME ", not including the filename." << endl
			<< endl << "Syntax Example -fc:\\pickit_2\\pk2cmd_dir" << endl;
}

void LongText::lx_c() {
	cout
			<< "Checks to see if the device is blank or not. Each memory region (Program,"
			<< endl
			<< "EEPROM, Configuration, and User ID memory) will be checked, and a message"
			<< endl
			<< "indicating whether or not the device is blank, will be displayed. If the"
			<< endl
			<< "device is not blank, the memory region and location of the first error"
			<< endl << "will be displayed." << endl << endl
			<< "This command takes no parameters." << endl << endl
			<< "Syntax Example -c" << endl;
}

void LongText::lx_d() {
	cout << "Upgrades the firmware on the programmer. This command must be done"
			<< endl << "independently of any other commands." << endl << endl
			<< "The parameter for this command is the complete file path to the .hex"
			<< endl << "file to be downloaded." << endl << endl
			<< "Syntax Example -dc:\\filepath\\PK2V021000.hex" << endl;
}

void LongText::lx_e() {
	cout << "Erases the device.  A warning will be issued if the device can"
			<< endl
			<< "only be bulk erased and VDD is below the bulk erase voltage."
			<< endl << endl << "This command takes no parameters." << endl
			<< endl << "Syntax Example -e" << endl;
}

void LongText::lx_f() {
	cout
			<< "Loads a hex file to the programmer. The device will not actually be"
			<< endl
			<< "programmed with the contents of the transferred hex file unless the"
			<< endl << "program command (-m) is also issued." << endl << endl
			<< "Binary format files are also supported for serial EEPROM devices only."
			<< endl
			<< "To load a binary file, the filename must end in BIN, ex: myfile.bin"
			<< endl << endl
			<< "The parameter for this command is the complete file path to the hex"
			<< endl << "file to be loaded" << endl << endl
			<< "Syntax Example -fc:\\filepath\\myfile.hex" << endl;
}

void LongText::lx_g() {
	cout << "Reads the device and outputs it to either the screen or a hexfile"
			<< endl
			<< "based on the type of read performed. The command must be immediately"
			<< endl
			<< "followed by the type of read, which can be one of the following:"
			<< endl
			<< "     f = Read into hex file. This command must be immediately followed"
			<< endl
			<< "         by the complete file path and name of the file to be created."
			<< endl
			<< "         Serial EEPROMs only may read into a binary file.  A binary file"
			<< endl
			<< "         will be created if the filename ends in BIN, ex: myfile.bin"
			<< endl
			<< "     p = Read program memory and output the result to the screen. This"
			<< endl
			<< "         command must be immediately followed by the hex address range"
			<< endl
			<< "         to be read, which must be in the form of x-y, where x = start"
			<< endl << "         address and y = end address." << endl
			<< "     e = Read EEData memory and output the result to the screen. This"
			<< endl
			<< "         command must be immediately followed by the hex address range"
			<< endl
			<< "         to be read, which must be in the form of x-y, where x = start"
			<< endl << "         address and y = end address." << endl
			<< "     i = Read User ID memory and output the result to the screen. No"
			<< endl
			<< "         further parameters are required for this command."
			<< endl
			<< "     c = Read Configuration memory and output the result to the screen."
			<< endl
			<< "         No further parameters are required for this command."
			<< endl
			<< "Multiple types of read commands can be included in the same command line."
			<< endl
			<< "NOTE: For HCS and serial EEPROM devices, memory is considered region 'P'"
			<< endl << endl << "Syntax Examples -gfc:\\filepath\\myfile" << endl
			<< "                -gp100-200" << endl
			<< "                -gi -ge0-40 -gc" << endl;
}

void LongText::lx_h() {
	cout
			<< "If this switch is included, PK2CMD will delay before exiting.  If the value "
			<< endl
			<< "is set to 'K', then PK2CMD will wait for a keypress before exiting. If the "
			<< endl
			<< "value is set to a number from 1 to 9, then it will delay the given number"
			<< endl << "of seconds before exiting." << endl << endl
			<< "The parameter for this command is the number of seconds (max = 9) to delay"
			<< endl
			<< "before exiting.  Parameter K will cause it to wait for a keypress."
			<< endl << endl << "Syntax Examples -h3" << endl
			<< "                -hk" << endl;
}

void LongText::lx_i() {
	cout
			<< "Reads and displays the value in the Device ID location of the device,"
			<< endl << "as well as the silicon revision code." << endl << endl
			<< "This will also display the device name that matches the returned Device ID,"
			<< endl
			<< "and warn if the Device ID does not match the device specified using the -p"
			<< endl << "command." << endl << endl
			<< "This command takes no parameters." << endl << endl
			<< "Syntax Example -i" << endl;
}

void LongText::lx_j() {
	cout
			<< "This switch will display a percentage completion for programming operations"
			<< endl
			<< "instead of the rotating slash.  If the switch is followed by the optional"
			<< endl
			<< "parameter 'n', then each percent update is printed on a newline.  This option"
			<< endl
			<< "is intended for GUI interfaces needing a newline to update the display."
			<< endl << endl
			<< "The optional parameter for this command, N, will print percentage updates"
			<< endl << "on a new line." << endl << endl << "Syntax Examples -j"
			<< endl << "                -jn" << endl;
}

void LongText::lx_l() {
	cout
			<< "Sets the ICSP clock (PGC) period to the given value, which must be a value"
			<< endl
			<< "between 1 and 16.  The value specifies the clock period in microseconds."
			<< endl
			<< "The default value is 1, which gives a period of 1us and an ICSP clock rate"
			<< endl
			<< "of 1 MHz.  A value of 2 gives a period of 2us and a clock rate of 500 kHz."
			<< endl
			<< "Slowing down the programming clock can help resolve programming issues with"
			<< endl
			<< "heavily loaded PGx lines and long programming cables.  A value of 4 usually"
			<< endl
			<< "resolves most such issues, but programming takes longer."
			<< endl << endl
			<< "The parameter for this command is a decimal value between 1 and 16 inclusive."
			<< endl << endl << "Syntax Example -l4" << endl;
}

void LongText::lx_k() {
	cout << "Displays the checksum of a loaded hexfile. This command must be"
			<< endl << "accompanied by the Hex File Selection command (-f)."
			<< endl << endl << "This command takes no parameters." << endl
			<< endl << "Syntax Example -k" << endl;
}

void LongText::lx_m() {
	cout << "Programs the device with the contents of the loaded hex file."
			<< endl
			<< "The entire device can be programmed, or just selected memory regions. If one"
			<< endl
			<< "or more selected regions are to be programmed, the program command must be"
			<< endl
			<< "immediately followed by a memory region parameter. Valid parameters are:"
			<< endl << "     P - program memory" << endl << "     E - EEPROM"
			<< endl << "     I - ID Memory" << endl
			<< "     C - Configuration Memory" << endl
			<< "If no memory region parameter is entered, the entire device will be erased and"
			<< endl
			<< "then programmed. Otherwise only the selected memory regions will be programmed"
			<< endl
			<< "without being first erased. Multiple program commands may be entered on one "
			<< endl
			<< "command line.\"Program Succeeded\" will be displayed if the operation is "
			<< endl
			<< "successful, otherwise the first address of the first memory region where "
			<< endl
			<< "programming failed will be displayed along with a description of the failure."
			<< endl
			<< "NOTE: For HCS and serial EEPROM devices, memory is considered region 'P'"
			<< endl << endl
			<< "This command may be used without parameters or with a memory region."
			<< endl << endl << "Syntax Examples -m" << endl
			<< "                -mP -mI -mE" << endl;
}

void LongText::lx_n() {
	cout
			<< "Assigns the given string to the PICkit 2 unit as the Unit ID.  The Unit ID is"
			<< endl
			<< "useful in uniquely identifying a PICkit 2 unit.  When multiple PICkit 2 units"
			<< endl
			<< "are connected to a PC, a specific PICkit 2 may be selected using the -S"
			<< endl << "option with the Unit ID. " << endl << endl
			<< "To assign a Unit ID to a PICkit 2, connect only that one unit to the PC and"
			<< endl
			<< "use this option.  To remove a Unit ID, do not include a string after the -N"
			<< endl
			<< "option.  A Unit ID may contain 14 characters maximum.  The Unit ID is stored"
			<< endl
			<< "in non-volatile memory in the PICkit 2 unit itself, and remains assigned"
			<< endl << "changed by a user." << endl << endl
			<< "Syntax Examples -nLab1B   (Set Unit ID = 'Lab1B')" << endl
			<< "                -n        (clear Unit ID)" << endl;
}

void LongText::lx_p() {
	cout << "There are three ways to use this option:" << endl
			<< "  1 : -P<part>" << endl
			<< "      Specify the part number of the device explicitly.  This is the"
			<< endl << "      recommended use.  Example: -pPIC16F887" << endl
			<< "  2 : -PF<id>" << endl
			<< "      Auto-Detect a target part connected to PICkit 2 within a given family."
			<< endl
			<< "      Use '-pf' for a list of auto-detectable families and their family ID"
			<< endl
			<< "      number.  Not all part families support detection.  No programming "
			<< endl
			<< "      operations are performed when -PF is used without an ID parameter."
			<< endl
			<< "      Use '-pf<id>' to auto-detect a part within a given family using"
			<< endl << "      the family ID from the listing.  Example: -pf2"
			<< endl << "  3 : -P" << endl
			<< "      Auto-Detect any part in all auto-detectable families when -p is"
			<< endl << "      is used with no parameters.  Example: -p" << endl
			<< endl
			<< "The -V and -X options may NOT be used with any form of auto-detect."
			<< endl
			<< "During auto-detect, VDD is ALWAYS 3.0 Volts unless -W is used.  After a part"
			<< endl
			<< "is detected, the device VDD default or -A voltage is used for remaining"
			<< endl << "operations." << endl << endl
			<< "Auto-detecting can be slower than explicitly specifying the part name."
			<< endl << endl
			<< "WARNING: SOME DEVICE FAMILIES USE A VPP VOLTAGE OF 12 VOLTS ON THE MCLR"
			<< endl
			<< "PIN.  THIS VOLTAGE MAY DAMAGE DEVICES FROM OTHER FAMILIES.  NEVER USE"
			<< endl
			<< "AN AUTODETECT OPTION ON A TARGET WITHOUT A KNOWN GOOD PROGRAMMING"
			<< endl
			<< "CONNECTION.  IT IS SAFER TO AUTO-DETECT WITHIN A GIVEN FAMILY (-PF) THAN"
			<< endl << "WITH ALL DETECTABLE FAMILIES." << endl << endl
			<< "Auto-detecting in all families goes through a special sequence of searching"
			<< endl
			<< "each family to prevent placing damaging voltages on parts.  However, if a"
			<< endl
			<< "programming connection problem prevents a part from being found, it may be"
			<< endl
			<< "exposed to damaging high voltages as other families are searched."
			<< endl << endl
			<< "PK2CMD -?P may be used to list all supported devices and their families."
			<< endl
			<< "PK2CMD -?P<str> may be used to list only devices matching the search string."
			<< endl;
}

void LongText::lx_q() {
	cout
			<< "Disables use of a Programming Executive (PE) for PIC24 or dsPIC33 devices."
			<< endl
			<< "Low-level ICSP is used instead (as in prior versions of PK2CMD)."
			<< endl << endl
			<< "Using the PE results in much faster programming operations, and implements"
			<< endl
			<< "the Device ID Corruption workaround for PIC24H/dsPIC33 devices.  However,"
			<< endl
			<< "Blank Check, Programming, and Verify operations will not provide the address"
			<< endl
			<< "and data for failing locations for PIC24H/dsPIC33 as the PE only returns a"
			<< endl
			<< "Good/Bad response.  Disable the PE for address and data information."
			<< endl << endl
			<< "The Programming Executive (PE) for PIC24H and dsPIC33F parts may fail on"
			<< endl
			<< "certain programming ports of certain 44-Pin devices. Known problems exist"
			<< endl << "with using the PGC3/PGD3 port on the following devices:"
			<< endl << "PIC24HJ16GP304, PIC24HJ32GP204" << endl
			<< "dsPIC33FJ16GP304, dsPIC33FJ32GP204, dsPIC33FJ16MC304, dsPIC33FJ32MC204"
			<< endl << endl << "Syntax Example -q" << endl;
}

void LongText::lx_r() {
	cout
			<< "Releases (3-states) the PICkit 2 /MCLR pin after programming operations"
			<< endl
			<< "complete.  If not specified, then /MCLR is asserted (driven low)."
			<< endl << endl << "There are no parameters for this command."
			<< endl << endl << "Syntax Example -r" << endl;
}

void LongText::lx_s() {
	cout
			<< "When more than one PICkit 2 unit is attached to a PC, this option allows"
			<< endl
			<< "a specific unit to be selected using its Unit ID.  The Unit ID is assigned"
			<< endl << "with the -N option." << endl << endl
			<< "When -S is used without an argument, all attached PICkit 2 units and their"
			<< endl
			<< "Unit IDs will be listed.  Units that do not have a Unit ID assigned will"
			<< endl
			<< "show a dash (-) in the Unit ID column.  When -S is used this way, all other"
			<< endl << "options will be ignored." << endl << endl
			<< "A single character argument of '#' may also be used.  This will list all"
			<< endl
			<< "PICkit 2 units with their Unit IDs and Firmware Versions.  NOTE that this"
			<< endl
			<< "is NOT safe to use when another application is already accessing a PICkit 2"
			<< endl
			<< "unit and may corrupt that USB connection. Also, a Unit ID of the single"
			<< endl
			<< "character '#' is not valid, but may used with longer IDs for example '#1'"
			<< endl << "or '#two' are valid." << endl << endl
			<< "To select a specific unit from among the attached PICkit 2 units to execute"
			<< endl
			<< "a given set of command options, follow the -S option with the Unit ID string"
			<< endl << "of the intended PICkit 2 unit." << endl << endl
			<< "This command may be used with or without a parameter." << endl
			<< endl
			<< "Syntax Example -s        (list connected PICkit 2 units - SAFE)"
			<< endl
			<< "               -s#       (list connected units with FW versions - UNSAFE)"
			<< endl
			<< "               -sLab1B   (use the PICkit 2 with Unit ID string 'Lab1B')"
			<< endl
			<< "               -#3       (use the PICkit 2 with Unit ID string '#3')"
			<< endl;
}

void LongText::lx_t() {
	cout
			<< "Enables the Vdd output pin after programming operations are complete."
			<< endl
			<< "If not specified, then Vdd is turned off.  Use -a<> to set the voltage."
			<< endl << endl << "There are no parameters for this command."
			<< endl << endl << "Syntax Example -t" << endl;
}

void LongText::lx_u() {
	cout
			<< "Specifies a new OSCCAL value in hex. Used with a Program command, the device"
			<< endl
			<< "will be programmed with this new value. No error checking is done on the value."
			<< endl << endl << "Syntax Example /uC80 or /u0x347C" << endl;
}

void LongText::lx_v() {
	cout
			<< "Specifies the Vpp value, in volts, that the device will be programmed with."
			<< endl
			<< "If not entered, the default value for the device is used.  Normally this"
			<< endl << "value should not be specified." << endl << endl
			<< "The parameter for this command is the floating point value of the desired"
			<< endl << "Vpp voltage." << endl << endl
			<< "Syntax Example -v13.00" << endl;
}

void LongText::lx_w() {
	cout
			<< "If this switch is included, the target circuit will not be powered by the "
			<< endl
			<< "programmer and should be powered by an external power source. If this switch"
			<< endl
			<< "is not included, the target circuit will be powered by the programmer. The"
			<< endl
			<< "PICkit 2 is limited to an external power source voltage range of 2.5 Volts"
			<< endl << "to 5.0 Volts." << endl << endl
			<< "There are no parameters for this command." << endl << endl
			<< "Syntax Example -w" << endl;
}

void LongText::lx_x() {
	cout
			<< "If this switch is included, PICkit 2 will attempt to program the device "
			<< endl
			<< "using the VPP first program entry method.  Not all families and devices"
			<< endl << "support this feature." << endl << endl
			<< "There are no parameters for this command." << endl << endl
			<< "Syntax Example -x" << endl;
}

void LongText::lx_y() {
	cout
			<< "Verifies the device against the selected hex file on the programmer."
			<< endl
			<< "The entire device can be verified, or just selected memory regions. If one"
			<< endl
			<< "or more selected regions are to be verified, the verify command must be"
			<< endl
			<< "immediately followed by a memory region parameter. Valid parameters are:"
			<< endl << "     P - program memory" << endl << "     E - EEPROM"
			<< endl << "     I - ID Memory" << endl
			<< "     C - Configuration Memory" << endl
			<< "If no memory region parameter is entered, the entire device will be verified,"
			<< endl
			<< "otherwise only the selected memory regions will be verified. Multiple verify"
			<< endl
			<< "commands may be entered on one command line. \"Verify Succeeded\" will be"
			<< endl
			<< "displayed if the operation is successful, otherwise the first address of the"
			<< endl
			<< "first memory region where verification failed will be displayed along with a"
			<< endl << "description of the failure." << endl
			<< "NOTE: For HCS and serial EEPROM devices, memory is considered region 'P'"
			<< endl << endl
			<< "This command may be used without parameters or with a memory region."
			<< endl << endl << "Syntax Examples -y" << endl
			<< "                -yP -yI -yE" << endl;
}

void LongText::lx_z() {
	cout
			<< "If this switch is included, then a complete device programming operation (-m)"
			<< endl
			<< "will preserve and not overwrite the existing EEPROM data memory on the device"
			<< endl << endl << "There are no parameters for this command."
			<< endl << endl << "Syntax Example -z" << endl;
}

void LongText::lx_display_help() {
	cout << "                        PICkit 2 COMMAND LINE HELP" << endl
			<< "Options              Description                              Default"
			<< endl
			<< "----------------------------------------------------------------------------"
			<< endl
			<< "A<value>             Set Vdd voltage                          Device Specific"
			<< endl
			<< "B<path>              Specify the path to " DEVICE_FILE_NAME "    Searches PATH"
			<< endl
			<< "                                                              and calling dir"
			<< endl
			<< "C                    Blank Check Device                       No Blank Check"
			<< endl
			<< "D<file>              OS Download                              None"
			<< endl
			<< "E                    Erase Flash Device                       Do Not Erase"
			<< endl
			<< "F<file>              Hex File Selection                       None"
			<< endl
			<< "G<Type><range/path>  Read functions                           None"
			<< endl << "                     Type F: = read into hex file,"
			<< endl << "                             path = full file path,"
			<< endl << "                             range is not used" << endl
			<< "                     Types P,E,I,C: = ouput read of Program,"
			<< endl
			<< "                             EEPROM, ID and/or Configuration"
			<< endl
			<< "                             Memory to the screen. P and E"
			<< endl
			<< "                             must be followed by an address"
			<< endl
			<< "                             range in the form of x-y where"
			<< endl
			<< "                             x is the start address and y is"
			<< endl
			<< "                             the end address both in hex,"
			<< endl << "                             path is not used" << endl
			<< "                             (Serial EEPROM memory is 'P')"
			<< endl
			<< "H<value>             Delay before Exit                        Exit immediately"
			<< endl
			<< "                         K = Wait on keypress before exit"
			<< endl << "                         1 to 9 = Wait <value> seconds"
			<< endl << "                                  before exit" << endl
			<< "I                    Display Device ID & silicon revision     Do Not Display"
			<< endl
			<< "J<newlines>          Display operation percent complete       Rotating slash"
			<< endl << "                         N = Each update on newline"
			<< endl
			<< "K                    Display Hex File Checksum                Do Not Display"
			<< endl
			<< "L<rate>              Set programming speed                    Fastest"
			<< endl
			<< "                     <rate> is a value of 1-16, with 1 being"
			<< endl << "                     the fastest." << endl
			<< "M<memory region>     Program Device                           Do Not Program"
			<< endl << "                     memory regions:" << endl
			<< "                         P = Program memory" << endl
			<< "                         E = EEPROM" << endl
			<< "                         I = ID memory" << endl
			<< "                         C = Configuration memory" << endl
			<< "                         If no region is entered, the entire"
			<< endl
			<< "                         device will be erased & programmed."
			<< endl
			<< "                         If a region is entered, no erase"
			<< endl
			<< "                         is performed and only the given"
			<< endl << "                         region is programmed." << endl
			<< "                         All programmed regions are verified."
			<< endl << "			            (serial EEPROM memory is 'P')" << endl
			<< "N<string>            Assign Unit ID string to first found     None"
			<< endl
			<< "                     PICkit 2 unit.  String is limited to 14"
			<< endl
			<< "                     characters maximum.  May not be used"
			<< endl << "                     with other options." << endl
			<< "                     Example: -NLab1B" << endl
			<< "P<part>              Part Selection. Example: -PPIC16f887     (Required)"
			<< endl
			<< "P                    Auto-Detect in all detectable families"
			<< endl << "PF                   List auto-detectable part families"
			<< endl
			<< "PF<id>               Auto-Detect only within the given part"
			<< endl
			<< "                     family, using the ID listed with -PF"
			<< endl << "                     Example: -PF2" << endl
			<< "Q                    Disable PE for PIC24/dsPIC33 devices     Use PE"
			<< endl
			<< "R                    Release /MCLR after operations           Assert /MCLR"
			<< endl
			<< "S<string/#>          Use the PICkit 2 with the given Unit ID  First found unit"
			<< endl
			<< "                     string.  Useful when multiple PICkit 2"
			<< endl << "                     units are connected." << endl
			<< "                     Example: -SLab1B" << endl
			<< "                     If no <string> is entered, then the"
			<< endl
			<< "                     Unit IDs of all connected units will be"
			<< endl
			<< "                     displayed.  In this case, all other "
			<< endl
			<< "                     options are ignored. -S# will list units"
			<< endl << "                     with their firmware versions."
			<< endl << "                     See help -s? for more info."
			<< endl
			<< "T                    Power Target after operations            Vdd off"
			<< endl
			<< "U<value>             Program OSCCAL memory, where:            Do Not Program"
			<< endl << "                      <value> is a hexadecimal number"
			<< endl
			<< "                      representing the OSCCAL value to be"
			<< endl
			<< "                      programmed. This may only be used in"
			<< endl << "                      conjunction with a programming "
			<< endl << "                      operation." << endl
			<< "V<value>             Vpp override                             Device Specific"
			<< endl
			<< "W                    Externally power target                  Power from Pk2"
			<< endl
			<< "X                    Use VPP first Program Entry Method       VDD first"
			<< endl
			<< "Y<memory region>     Verify Device                            Do Not Verify"
			<< endl << "                         P = Program memory" << endl
			<< "                         E = EEPROM" << endl
			<< "                         I = ID memory" << endl
			<< "                         C = Configuration memory" << endl
			<< "                         If no region is entered, the entire"
			<< endl << "                         device will be verified."
			<< endl << "                         (Serial EEPROM memory is 'P')"
			<< endl
			<< "Z                    Preserve EEData on Program               Do Not Preserve"
			<< endl
			<< "?                    Help Screen                              Not Shown"
			<< endl << endl
			<< "     Each option must be immediately preceeded by a switch, Which can"
			<< endl
			<< "     be either a dash <-> or a slash </> and options must be separated"
			<< endl << "     by a single space." << endl << endl
			<< "     Example:   PK2CMD /PPIC16F887 /Fc:\\mycode /M" << endl
			<< "                               or" << endl
			<< "                PK2CMD -PPIC16F887 -Fc:\\mycode -M" << endl
			<< endl
			<< "     Any option immediately followed by a question mark will invoke"
			<< endl
			<< "     a more detailed description of how to use that option."
			<< endl << endl
			<< "     Commands and their parameters are not case sensitive. Commands will"
			<< endl
			<< "     be processed according to command order of precedence, not the order"
			<< endl << "     in which they appear on the command line. " << endl
			<< "	Precedence:" << endl << "                -?      (first)"
			<< endl << "                -B" << endl << "                -S"
			<< endl << "                -D" << endl << "                -N"
			<< endl << "                -P" << endl
			<< "                -A -F -J -L -Q -V -W -X -Z" << endl
			<< "                -C" << endl << "                -U" << endl
			<< "                -E" << endl << "                -M" << endl
			<< "                -Y" << endl << "                -G" << endl
			<< "                -I -K" << endl << "                -R -T"
			<< endl << "                -H      (last)" << endl << "		" << endl
			<< "     The program will return an exit code upon completion which will"
			<< endl
			<< "     indicate either successful completion, or describe the reason for"
			<< endl
			<< "     failure. To view the list of exit codes and their descriptions,"
			<< endl << "     type -?E on the command line." << endl << endl
			<< "     type -?V on the command line for version information."
			<< endl << endl
			<< "     type -?L on the command line for license information."
			<< endl << endl
			<< "     type -?P on the command line for a listing of supported devices."
			<< endl
			<< "     type -?P<string> to search for and display a list of supported devices"
			<< endl << "                      beginning with <string>." << endl
			<< endl
			<< "     Special thanks to the following individuals for their critical"
			<< endl << "     contributions to the development of this software:"
			<< endl << "		Jeff Post, Xiaofan Chen, and Shigenobu Kimura"
			<< endl;
}

void LongText::lx_display_license() {
	cout << "IMPORTANT: " << endl
			<< "YOU MUST ACCEPT THE TERMS AND CONDITIONS OF THIS LICENSE AGREEMENT"
			<< endl
			<< "TO RECEIVE A LICENSE FOR THE ACCOMPANYING SOFTWARE.  TO ACCEPT THE"
			<< endl
			<< "TERMS OF THIS LICENSE, OPEN THIS PACKAGE AND PROCEED WITH THE"
			<< endl
			<< "DOWNLOAD OR USE OF THE SOFTWARE.  IF YOU DO NOT ACCEPT THESE LICENSE"
			<< endl
			<< "TERMS, DO NOT OPEN THIS PACKAGE, DOWNLOAD, OR USE THIS SOFTWARE."
			<< endl << endl << "PICkit(tm) 2 PK2CMD SOFTWARE LICENSE " << endl
			<< endl
			<< "This License Agreement (Agreement) is a contract between You (as"
			<< endl
			<< "an individual or as a representative of your employer) and"
			<< endl
			<< "Microchip Technology Incorporated (\"Company\") for the PICkit(tm) 2"
			<< endl
			<< "PK2CMD software (including source code) accompanying this Agreement"
			<< endl
			<< "(the \"Software\").  In consideration for access to the Software, You"
			<< endl << "agree to be bound by this Agreement. " << endl << endl
			<< "1.  LICENSE GRANT. Subject to all of the terms and conditions of"
			<< endl
			<< "this Agreement, Company grants You a non-exclusive, non-"
			<< endl
			<< "sublicensable, non-transferable license to use the Software with"
			<< endl
			<< "Company products, modify the Software for use with Company products,"
			<< endl << "and market, sell or otherwise distribute: " << endl
			<< endl
			<< "(a) Your end application that integrates Software and Company"
			<< endl << "    products (\"Licensee Product\"); or " << endl
			<< endl
			<< "(b) Your modifications to the Software provided that the modified"
			<< endl
			<< "    Software has the following copyright and disclaimer notice"
			<< endl
			<< "    prominently posted in a location where end users will see it"
			<< endl
			<< "    (e.g., installation program, program headers, About Box, etc.):"
			<< endl << endl
			<< "\"Copyright (c) 2005-2009 Microchip Technology Inc. All rights"
			<< endl
			<< "reserved. This version of the PICkit(tm) 2 PK2CMD Software has been"
			<< endl
			<< "modified by [INSERT YOUR NAME, DATE OF SOFTWARE MODIFICATION HERE]."
			<< endl
			<< "You may use, copy, modify and distribute the Software for use with"
			<< endl
			<< "Microchip products only.  If you distribute the Software or its"
			<< endl
			<< "derivatives, the Software must have this copyright and disclaimer"
			<< endl
			<< "notice prominently posted in a location where end users will see it"
			<< endl
			<< "(e.g., installation program, program headers, About Box, etc.).  To"
			<< endl
			<< "the maximum extent permitted by law, this Software is distributed"
			<< endl
			<< "\"AS IS\" and WITHOUT ANY WARRANTY INCLUDING BUT NOT LIMITED TO ANY"
			<< endl
			<< "IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR PARTICULAR PURPOSE,"
			<< endl
			<< "or NON-INFRINGEMENT. IN NO EVENT WILL MICROCHIP OR ITS LICENSORS BE"
			<< endl
			<< "LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL"
			<< endl
			<< "DAMAGESOF ANY KIND ARISING FROM OR RELATED TO THE USE, MODIFICATION"
			<< endl << "OR DISTRIBUTION OF THIS SOFTWARE OR ITS DERIVATIVES.\""
			<< endl << endl
			<< "You may not copy or reproduce all or any portion of Software, except"
			<< endl
			<< "to the extent that such activity is specifically allowed by this"
			<< endl
			<< "Agreement or expressly permitted by applicable law notwithstanding"
			<< endl << "the foregoing limitations." << endl << endl
			<< "All copies of the Software created by You or for You, including"
			<< endl
			<< "derivatives, must include the copyright, trademark and other"
			<< endl
			<< "proprietary notices as they appear on the original or, in the event"
			<< endl
			<< "You modified the Software, the notice listed above. You may not"
			<< endl
			<< "remove or alter any identifying screen that is produced by the"
			<< endl << "Software." << endl << endl
			<< "2.  OWNERSHIP AND TITLE. Software is licensed pursuant to the"
			<< endl
			<< "    Agreement, not sold.  All right, title and interest, including"
			<< endl
			<< "    intellectual property rights, in and to Software, derivatives"
			<< endl
			<< "    thereof, implementation of the Software in microcontrollers,"
			<< endl
			<< "    and hardware and software implementations of Software or"
			<< endl
			<< "    derivatives shall remain in Company. You will not obtain"
			<< endl
			<< "    ownership rights to derivatives of Software, and by accepting"
			<< endl
			<< "    the terms of this Agreement assign any such rights to Company"
			<< endl
			<< "    that You do receive.  Except as specifically stated in the"
			<< endl
			<< "    Agreement, you are granted no other rights, express or implied,"
			<< endl
			<< "    to the Software, derivatives thereof, or other Company"
			<< endl
			<< "    intellectual property such as trade secrets, patents, "
			<< endl << "    copyrights, and trademarks." << endl << endl
			<< "3.  CONFIDENTIALITY. You agree not to disclose Software to any"
			<< endl
			<< "    third party, except as permitted by this Agreement.  To the"
			<< endl
			<< "    extent that Software becomes part of the public domain, is"
			<< endl
			<< "    independently developed, or obtained free from any obligation"
			<< endl
			<< "    of confidentiality then the obligation of confidentiality"
			<< endl << "    under this Agreement shall not apply." << endl
			<< endl
			<< "4.  COPYRIGHT. The Software is protected by U.S. copyright laws"
			<< endl
			<< "    and international copyright treaties, as well as other"
			<< endl << "    intellectual property laws and treaties." << endl
			<< endl
			<< "5.  TERMINATION OF AGREEMENT. Without prejudice to any other"
			<< endl
			<< "    rights, Company may terminate this Agreement if You fail to"
			<< endl
			<< "    comply with the terms and conditions of this Agreement."
			<< endl
			<< "    Upon termination, You shall immediately: (a) stop using and"
			<< endl
			<< "    distributing the Software and derivatives thereof; (b) destroy"
			<< endl
			<< "    all copies of the Software and derivatives in your possession;"
			<< endl
			<< "    and (c) remove Software from any of Your tangible media and"
			<< endl
			<< "    from systems on which the Software exists.  Termination of"
			<< endl
			<< "    this License shall not affect the right of any end user or"
			<< endl
			<< "    consumer to use Licensee Product or modified Software;"
			<< endl
			<< "    provided that such product or modified Software was purchased"
			<< endl
			<< "    or distributed prior to the termination of this License."
			<< endl << endl
			<< "6.  DANGEROUS APPLICATIONS. You acknowledge that Software has not"
			<< endl
			<< "    been designed to be fault tolerant.  You warrant that You will"
			<< endl
			<< "    not use Software or derivatives in a dangerous, hazardous, or"
			<< endl
			<< "    life supporting application where the failure of such"
			<< endl
			<< "    application could lead directly to death, personal injury, or"
			<< endl << "    environmental damage." << endl << endl
			<< "7.  INDEMNITY. You will indemnify and hold Company and its"
			<< endl
			<< "    licensor(s), its related companies and its suppliers, harmless"
			<< endl
			<< "    for, from and against, any claims, costs (including attorney's"
			<< endl
			<< "    fees), damages or liabilities, including without limitation"
			<< endl
			<< "    product liability claims, arising out of: (a) Your use,"
			<< endl
			<< "    modification and distribution of the Software and its"
			<< endl
			<< "    derivatives; or (b) violation of this Agreement. COMPANY AND"
			<< endl
			<< "    ITS LICENSOR(S) ASSUME NO RESPONSIBILITY FOR, NOR INDEMNIFY"
			<< endl
			<< "    YOU AGAINST, ANY PATENT, COPYRIGHT OR OTHER INTELLECTUAL"
			<< endl
			<< "    PROPERTY CLAIMS BROUGHT AGAINST YOU RELATING TO THE SOFTWARE."
			<< endl << endl
			<< "8.  NO WARRANTY. TO THE MAXIMUM EXTENT PERMITTED BY LAW, COMPANY"
			<< endl
			<< "    AND ITS LICENSOR PROVIDE SOFTWARE \"AS IS\" AND EXPRESSLY"
			<< endl
			<< "    DISCLAIM ANY WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED,"
			<< endl
			<< "    INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF"
			<< endl
			<< "    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR"
			<< endl
			<< "    NON-INFRINGEMENT. YOU ASSUME THE ENTIRE RISK ARISING OUT OF"
			<< endl
			<< "    USE OR PERFORMANCE OF SOFTWARE, AS WELL AS ANY DERIVATIVES OF"
			<< endl
			<< "    THE SOFTWARE MADE FOR YOU OR ON YOUR BEHALF.  COMPANY AND ITS"
			<< endl
			<< "    LICENSOR(S) ASSUME NO RESPONSIBILITY FOR THE ACCURACY OR"
			<< endl
			<< "    ERRORS OR OMISSIONS OF SOFTWARE AND DO NOT WARRANT THE"
			<< endl
			<< "    FOLLOWING: (A) THE FUNCTIONS CONTAINED IN SOFTWARE WILL MEET"
			<< endl
			<< "    YOUR REQUIREMENTS; (B) THE OPERATION OF SOFTWARE WILL BE"
			<< endl
			<< "    UNINTERRUPTED OR ERROR-FREE; OR (C) ANY DEFECTS IN SOFTWARE"
			<< endl << "    WILL BE CORRECTED. " << endl << endl
			<< "9.  LIMITATION OF LIABILITY. COMPANY AND ITS LICENSOR TOTAL"
			<< endl
			<< "    AGGREGATE LIABILITY IN CONTRACT, WARRANTY, TORT (INCLUDING"
			<< endl
			<< "    NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,"
			<< endl
			<< "    INDEMNITY, CONTRIBUTION, OR OTHERWISE, SHALL NOT EXCEED THE"
			<< endl
			<< "    LICENSE FEE YOU PAID FOR THE SOFTWARE. IN NO EVENT SHALL"
			<< endl
			<< "    COMPANY AND ITS LICENSOR BE LIABLE FOR ANY INCIDENTAL, SPECIAL,"
			<< endl
			<< "    INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,"
			<< endl
			<< "    HARM TO YOUR EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE"
			<< endl
			<< "    GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS BY THIRD PARTIES"
			<< endl
			<< "    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), ANY CLAIMS"
			<< endl
			<< "    FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS. If any"
			<< endl
			<< "    remedy is determined to have failed of its essential purpose,"
			<< endl
			<< "    all limitations of liability and exclusion of damages set forth"
			<< endl << "    in the limited warranty shall remain in effect."
			<< endl << endl
			<< "10. SURVIVAL.  Sections 2-15 shall survive termination of this"
			<< endl << "     Agreement. " << endl << endl
			<< "11. CHOICE OF LAW; VENUE; LIMITATIONS ON CLAIMS. You agree that"
			<< endl
			<< "    this Agreement and any conflicts regarding Software, shall be"
			<< endl
			<< "    construed, interpreted and governed by the laws, and subject"
			<< endl
			<< "    to the exclusive jurisdiction of the state or territory in the"
			<< endl
			<< "    Company Terms and Conditions of Sale (\"T&Cs\").  In the event"
			<< endl << "    that the T&Cs do not apply or the choice of law or"
			<< endl
			<< "    jurisdiction are not indicated therein, then this Agreement"
			<< endl
			<< "    shall be construed, interpreted and governed by the laws, and"
			<< endl
			<< "    subject to the exclusive jurisdiction of the State of Arizona,"
			<< endl
			<< "    U.S.A. without regard to any conflict of laws principles. You"
			<< endl
			<< "    agree that regardless of any law to the contrary, any cause of"
			<< endl
			<< "    action related to or arising out of this Agreement or Software"
			<< endl
			<< "    must be filed within one year after such cause of action"
			<< endl << "    arose, or be considered waived." << endl << endl
			<< "12. EXPORT COMPLIANCE. You will not export or re-export Software,"
			<< endl
			<< "    technical data, direct products thereof or any other items"
			<< endl
			<< "    which would violate any applicable export control laws and"
			<< endl
			<< "    regulations including, but not limited to, those of the United"
			<< endl
			<< "    States and the United Kingdom.  You agree that it is Your"
			<< endl
			<< "    responsibility to obtain copies of and to familiarize yourself"
			<< endl
			<< "    fully with these laws and regulations to avoid violation."
			<< endl << endl
			<< "13. ASSIGNMENT. Neither this agreement nor any rights, licenses"
			<< endl
			<< "    or obligations hereunder, may be assigned by You without the"
			<< endl << "    Company's prior written approval." << endl << endl
			<< "14. ENTIRE AGREEMENT: MODIFICATIONS AND WAIVER. This Agreement"
			<< endl
			<< "    constitutes the entire agreement of the parties with respect"
			<< endl
			<< "    to the subject matter of this Agreement, and merges and"
			<< endl
			<< "    supersedes all communications relating to this subject matter,"
			<< endl
			<< "    whether written or oral. Except as expressly set forth in this"
			<< endl
			<< "    Agreement, no modification of this Agreement will be effective"
			<< endl
			<< "    unless made in writing signed by Company.  No failure or delay"
			<< endl
			<< "    by Company or its licensor(s) to assert any rights or remedies"
			<< endl
			<< "    arising from a breach of this Agreement shall be construed as a"
			<< endl
			<< "    waiver or a continuing waiver of such rights and remedies, nor"
			<< endl
			<< "    shall failure or delay to assert a breach be deemed to waive that"
			<< endl
			<< "    or any other breach. If any part of this Agreement is found by a"
			<< endl
			<< "    court of competent jurisdiction to be invalid, unlawful or"
			<< endl
			<< "    unenforceable then such part shall be severed from the remainder"
			<< endl
			<< "    of this Agreement and replaced with a valid provision that comes"
			<< endl
			<< "    closest to the intention underlying the invalid provision."
			<< endl << endl
			<< "Copyright (c) 2005-2009, Microchip Technology Inc. All rights"
			<< endl << "reserved. " << endl;
}

void LongText::lx_partlist_notice() {
	cout << endl
			<< "NOTE: If the name contains a space with additional info in parentheses, Ex:"
			<< endl << "        PIC16F636 (639)" << endl
			<< "        93LC46A (C X8)" << endl
			<< "      then only the characters before the space are required for -P, Ex:"
			<< endl << "        -pPIC16F636" << endl << "        -p93LC46A"
			<< endl;
}

