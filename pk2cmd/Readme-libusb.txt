

Linux only:

You will need libusb runtime to run pk2cmd.  Typical linux distros will have
libusb-0.1 package installed already, so no additonal installation is
needed. If not, use your linux distro's package management software to
install it (typically named libusb).

The libusb-0.1 development package is needed to build pk2cmd from source. However,
by default, the development package is not installed by the distros. Therefore
you need to install it. It is typically called libusb-dev or libusb-devel.

If you are running a very old Linux version which has a version of libusb older
than 0.1.10, then it is better that you install libusb-0.1 from source. In this
case, you will have both the runtime library and the development package
installed at the same time.

You can download libusb-0.1 from http://libusb.sourceforge.net. See the README
and INSTALL files for instructions on how to install libusb on your system.

