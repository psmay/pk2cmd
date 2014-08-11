#
# Makefile for pk2cmd for Linux/OSX
#
# To build for Linux, uncomment one of the CFLAGS options, then type 'make'.
# To build on OS/X, select the appropriate CFLAGS and type 'make osx' or 'make osxhid'.
#
# You MUST select a CFLAGS below in order to compile.
#
# Alternatively you can select a CFLAGS by specifying the target architecture
# on the command line, such as:
#    make linux
# or
#    make mac
#

APP=pk2cmd
CC=g++
# Uncomment this for debugging with gdb
#DBG=-ggdb -O0
# Uncomment this for no debugging (default)
DBG=-O2
OPTS=-Wall -D_GNU_SOURCE $(DBG)
OBJS=cmd_app.o DeviceData.o DeviceFile.o ImportExportHex.o PICkitFunctions.o PIC32PE.o Pk2BootLoader.o pk2cmd.o stdafx.o pk2usbcommon.o pk2usb.o P24F_PE.o dsP33_PE.o strnatcmp.o

# Grab the version of libusb in /usr/local if possible:
LIBUSB=/usr/local
LDFLAGS=-L$(LIBUSB)/lib -lusb

# Flags for linux
CFLAGS=$(OPTS) -I$(LIBUSB)/include -DLINUX -DUSE_DETACH -DCLAIM_USB -UDARWIN -UMACOSX_HID -UMACOSX105

.PHONY: all

all: $(APP)
#	@echo $(HOME)

$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) 

cmd_app.o: cmd_app.cpp cmd_app.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

DeviceData.o: DeviceData.cpp DeviceData.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

DeviceFile.o: DeviceFile.cpp DeviceFile.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

ImportExportHex.o: ImportExportHex.cpp ImportExportHex.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

PICkitFunctions.o: PICkitFunctions.cpp PICkitFunctions.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

PIC32PE.o: PIC32PE.cpp PIC32PE.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

Pk2BootLoader.o: Pk2BootLoader.cpp Pk2BootLoader.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

pk2cmd.o: pk2cmd.cpp cmd_app.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

stdafx.o: stdafx.cpp stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

pk2usbcommon.o: pk2usbcommon.cpp pk2usb.h
	$(CC) $(CFLAGS) -o $@  -c $<

pk2usb.o: pk2usb.cpp pk2usb.h
	$(CC) $(CFLAGS) -o $@  -c $<

P24F_PE.o: P24F_PE.cpp P24F_PE.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

dsP33_PE.o: dsP33_PE.cpp dsP33_PE.h stdafx.h
	$(CC) $(CFLAGS) -o $@  -c $<

strnatcmp.o: strnatcmp.c strnatcmp.h stdafx.h
	$(CC) $(CFLAGS) -x c -o $@  -c $<

install: 
	mkdir -p /usr/share/pk2
	cp $(APP) /usr/local/bin
	chmod u+s /usr/local/bin/$(APP)
	cp PK2DeviceFile.dat /usr/share/pk2/PK2DeviceFile.dat

clean:
	rm -f *.o
	rm -f $(APP)



