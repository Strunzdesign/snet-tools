# Tools to work with s-net(r) devices (snet-tools)
A set of tools to exchange and handle packets of s-net(r) devices via the HDLC Daemon.

This package offers a set of tools to exchange messages with s-net(r) devices of the Fraunhofer-Institute for Integrated Circuits (IIS).
Besides multiple tools, it includes header files to ease creation of own tools. This package depends on the "HDLC Daemon" (HDLCd)
that implements the "High-level Data Link Control" protocol (HDLC). The HDLC-Daemon is found here:
- https://github.com/Strunzdesign/hdlcd

This software is intended to be portable and makes use of the boost libraries. It was tested on GNU/Linux (GCC toolchain)
and Microsoft Windows (nuwen MinGW).

## Latest stable release of the s-net(r) tool suite:
- v1.1 from 06.10.2016 
  - Multiple bug fixes and stability improvements
  - Makes use of git submodules for "externals"

## Required libraries and tools:
- GCC, the only tested compiler collection thus far (tested: GCC 4.9.3, GCC 6.1)
- Boost, a platform-independent toolkit for development of C++ applications
- CMake, the build system
- Doxygen, for development
- nuwen MinGW, to compile the software on Microsoft Windows (tested: 13.4, 14.0)

## Documentation
- See online doxygen documentation at http://strunzdesign.github.io/snet-tools/
- Check the change log at https://github.com/Strunzdesign/snet-tools/blob/master/CHANGELOG.md
- View the install instructions at https://github.com/Strunzdesign/snet-tools/blob/master/INSTALL
- Have a look at an overview of the provided tools at https://github.com/Strunzdesign/snet-tools/blob/master/doc/BINARIES.txt
- Read the HOWTO at https://github.com/Strunzdesign/snet-tools/blob/master/doc/HOWTO.txt
