//
// A USB interface to the Microchip(tm) PICkit(tm) 2 FLASH Starter Kit
// device programmer and breadboard.
// PIC, PICkit2 are registered trademarks of Microchip, Inc.
//
// By Jeff Post, j_post <AT> pacbell <DOT> net
//
//-----------------------------------------------------------------------------
//
//	This program is free software; you can redistribute it and/or
//	modify it under the terms of the GNU General Public License
//	as published by the Free Software Foundation; either version 3
//	of the License, or (at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//-----------------------------------------------------------------------------
//

#ifndef _PK2USB_H
#define _PK2USB_H

// To enable USB debugging info, set USB_DEBUG_FLAGS to a combination of any of
// the following:

static const int	USB_DEBUG_XMIT =		1; // display transmitted data
static const int	USB_DEBUG_RECV =		2; // display received data
static const int	USB_DEBUG_CMD =			(4 | USB_DEBUG_XMIT); // interpret commands in display
static const int	USB_DEBUG_SCRIPT =		(8 | USB_DEBUG_XMIT); // interpret scripts in display
static const int	USB_DEBUG_DRIVER =		0x10; // enable libusb debugging info (Linux only)
static const int	USB_DEBUG_FULL =		(USB_DEBUG_RECV | USB_DEBUG_CMD | USB_DEBUG_SCRIPT);
static const int	USB_DEBUG_MAX =			(USB_DEBUG_RECV | USB_DEBUG_CMD | USB_DEBUG_SCRIPT | USB_DEBUG_DRIVER);

// Examples:
//#define	USB_DEBUG_FLAGS	(USB_DEBUG_XMIT | USB_DEBUG_RECV)
//#define	USB_DEBUG_FLAGS	USB_DEBUG_FULL

static const int	USB_DEBUG_FLAGS =	0; // No USB debugging by default

#include	<usb.h>	// Linux
typedef struct usb_dev_handle pickit_dev;
extern usb_dev_handle	*deviceHandle;

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;

static const int	reqLen =			64; // PICkit 2 always uses 64-byte transfers

static const int	CONFIG_HID =		1; // Use HID for pickit configuration
static const int	CONFIG_VENDOR =		2; // Vendor specific configuration

static const int	NORMAL_MODE =		1;
static const int	BOOTLOAD_MODE =		2;

static const int	AUTOPOWER =			0; // auto detect target power
static const int	PK2POWER =			1; // power target from PICkit2
static const int	SELFPOWER =			2; // target is self-powered

static const int	MINFWVERSION =	0x020a00;

// Bootloader commands

static const int	READFWFLASH =		1;
static const int	WRITEFWFLASH =		2;
static const int	ERASEFWFLASH =		3;
static const int	READFWEEDATA =		4;
static const int	WRITEFWEEDATA =		5;
static const int	NOOPERATION =		'Z';
static const int	RESETFWDEVICE =		0xff;

// PICkit2 commands (version 2 firmware)

static const int	ENTERBOOTLOADER =	'B'	; // 0x42 - version 1 and version 2
static const int	POWERCTRL =			'V'	; // 0x56
static const int	GETVERSION =		'v'	; // 0x76 - version 1 and version 2
static const int	SETVDD =			0xa0;
static const int	SETVPP =			0xa1;
static const int	READ_STATUS =		0xa2;
static const int	READ_VOLTAGES =		0xa3;
static const int	DOWNLOAD_SCRIPT =	0xa4;
static const int	RUN_SCRIPT =		0xa5; // run script from buffer
static const int	EXECUTE_SCRIPT =	0xa6; // run included script
static const int	CLR_DOWNLOAD_BFR =	0xa7;
static const int	DOWNLOAD_DATA =		0xa8;
static const int	CLR_UPLOAD_BFR =	0xa9;
static const int	UPLOAD_DATA =		0xaa;
static const int	CLR_SCRIPT_BFR =	0xab;
static const int	UPLOAD_DATA_NOLEN =	0xac;
static const int	END_OF_BFR =		0xad;
static const int	RESET =				0xae;
static const int	SCRIPT_BFR_CHKSM =	0xaf;
static const int	WR_INTERNAL_EE =	0xb1;
static const int	RD_INTERNAL_EE =	0xb2;

// Script commands
static const int	BUSY_LED_OFF =			0xf4;
static const int	BUSY_LED_ON =			0xf5;
static const int	MCLR_GND_OFF =			0xf6;
static const int	MCLR_GND_ON =			0xf7;
static const int	VPP_PWM_OFF =			0xf8;
static const int	VPP_PWM_ON =			0xf9;
static const int	VPP_OFF =				0xfa;
static const int	VPP_ON =				0xfb;
static const int	VDD_GND_OFF =			0xfc;
static const int	VDD_GND_ON =			0xfd;
static const int	VDD_OFF =				0xfe;
static const int	VDD_ON =				0xff;

extern bool	verbose;
extern int	pickit_interface;
extern int	pickit2mode;
extern int	usbdebug;
extern int	pickit2firmware;
extern int	targetPower;
extern FILE	*usbFile;
extern byte	cmd[reqLen + 1];

#include "usbhidioc.h"

struct scriptInterpreter {
	byte	scriptcode;	// opcode in script buffer
	byte	args;			// number of arguments (255 = variable, first arg is count, 254 = script #, then count)
	char	*name;		// mnemonic for script
};

extern struct scriptInterpreter scriptInterpreter[];
extern char	*scriptNames[];

extern int	indexScriptInterpreter(byte code);
extern void	showUSBCommand(byte *src, int len);
extern void	sendPickitCmd(pickit_dev *d, byte *src, int len);
extern void	sendScriptCmd(pickit_dev *d, byte *src, int len);
extern void	enableTargetPower(pickit_dev *d);
extern void	disableTargetPower(pickit_dev *d);
extern void	pickitOn(pickit_dev *d);
extern void	pickitOff(pickit_dev *d);

extern bool	FindTheHID(int unitIndex);
extern bool	ReadReport(char InBuffer[]);
extern bool	WriteReport(char OutputBuffer[], unsigned int nBytes);
extern void	CloseReport(void);

// Send data over usb
extern int sendUSB(pickit_dev *d, byte *src, int len);

// Read data from usb
extern int recvUSB(pickit_dev *d, int len, byte *dest);

// Open the pickit as a usb device.  Aborts on errors.
extern pickit_dev *usbPickitOpen(int unitIndex, char *id);


#endif  // _PK2USB_H

