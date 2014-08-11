Building pk2cmd in Linux and Mac OS X
pk2cmd version 1.10.00 - June 2008
-------------------------------------

A Makefile is provided for compiling on several versions of Linux and Mac OS X.
Select the system either by uncommenting one of the TARGET options in the
Makefile or by specifying the target on the command line.

make linux              Build for Linux kernel 2.6
make linux-old          Build for Linux kernel 2.4
make freebsd            Build for FreeBSD	(See FREEBSD NOTE below)
make mac104             Build for OS X version 10.4
make mac105             Build for OS X version 10.5

After successfully compiling pk2cmd, use su or sudo to run 'make install' as
root. The pk2cmd binary will be copied to /usr/local/bin with the 's'
permission bit set (necessary for running pk2cmd as a non-root user). The
device data file, PK2DeviceFile.dat, will be copied to /usr/share/pk2. If
/usr/share/pk2 doesn't already exist, it will be created by running
make install.

Both of these directories must be in your PATH variable. /usr/local/bin should
normally be in your PATH by default. If you're using bash as your command line
interface, edit the file .bashrc in your home directory to include:
  PATH=$PATH:/usr/share/pk2
  export PATH
Some similar procedure should exist for other command line shells; consult your
system's documentation. You'll need to restart your window manager for these
changes to take effect.

On Mac OS X, tcsh cannot be used because the '?' character has special meaning.
You will need to run pk2cmd using bash.

If compiling for Linux kernel 2.4, you will get some compiler warnings stating
that '#pragma once is obsolete'. These warnings can be safely ignored.


Please read the file 'usbhotplug.txt' before compiling and installing pk2cmd.
It explains how to set up USB udev or hotplug rules so pk2cmd can be run in
user mode. You must also have libusb (http://libusb.sourceforge.net) installed
on your system.


FREEBSD NOTE:
Due to USB stack problems in FreeBSD, the PK2CMD implementation in this OS
is still considered very "experimental."

For help in getting pk2cmd to work under FreeBSD, try referring to the 
following blog posts by Xiaofan Chen:
http://mcuee.blogspot.com/2007/11/pk2cmd-ported-to-linux.html
http://mcuee.blogspot.com/2007/11/setting-up-permissions-for-usb-ports-to.html

The test was based on FreeBSD 7.0 Release version but it may be possible to
replicate the process with other FreeBSD versions.

The following are known problems in FreeBSD, even when the program is otherwise
"working":

The USB stack issues prevent pk2cmd from being able to successfully update the
PICkit 2 operating system firmware.  Make sure the PICkit 2 unit firmware is
up-to-date using another OS (such as Linux, Windows, or MacOS).

Support for multiple programmers using the '-s' options does not work under
FreeBSD either.  Do not use the '-s' option on this OS.  PK2CMD should work
using the first PICkit 2 unit it finds.
