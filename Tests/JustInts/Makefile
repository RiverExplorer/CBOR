#
# Project: Phoenix
# Time-stamp: <2025-05-28 19:46:58 doug>
#
# Copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
# Author Douglas Mark Royer
# @date 17-APR-20205
#
# licensed under CC BY 4.0.
#
# RiverExplorer is a trademark of Douglas Mark Royer
#

# Version of this program/library
#
.SUFFIXES: .g4 .cpp

VERSION			= 0.0.1
VENDER			= RiverExplorer

# TOP of the build tree where BuildTools/Make... files exist
#
TOP					= ../..
LIBTOP			= ../..

# What to call this program.
# And what the include/RiverExplorer/$(TARGET) header directory will be.
#
TARGET			= JustInts

NOW = $(shell date +%Y%m%dT%H%M%S)

LINUX_CPPFLAGS	+= \
	-I/usr/include/tirpc \
	-DTARG_BINARY_s='"$(TARGET)"' \
	-DFULLVERSION_s='"$(NOW)"' \
	-DOS_s='"Linux"' \
	-DOSVER_s='"$(shell uname -r)"'


include $(TOP)/BuildTools/Makefile.top

CXX_VERSION = -std=c++23

SOURCES.cpp = \
	JustInts.cpp

SOURCES.hpp =

W64_CPPFLAGS	+=

ANDROID_CPPFLAGS += 

LINUX_LIBS		+= -L../../BuildTemp/Linux/Debug/lib
W64_LIBs			+= -L../../BuildTemp/W64/Debug/lib
ANDROID_LIBS	+= -L../../BuildTemp/Android/Debug/lib

CPPFLAGS	+= \
	-I../.. \
	-I/usr/include/antlr4-runtime \
	-I/usr/include/tirpc \
	-I$(ANTLR4_DIRECTORY)

CXXFLAGS	+= -ggdb -std=c++23 -mfloat128
LDFLAGS		+= -ggdb -std=c++23 -mfloat128

LIBS		+= -lCBOR -lcppunit

# Build as much as possible, with static libs.
#
SLIBS		+= $(LIBS)

Default:
	@echo "Use one of:"
	@echo "  make LinuxDebug"
	@echo "  make LinuxRelease"
	@echo "  make LinuxTcov"
	@echo "  make Linux - To make LinuxDebug, LinuxRelease, and LinuxTcov"
	@echo ""
	@echo "  make W64Debug"
	@echo "  make W64Tcov"
	@echo "  make W64Releaes"
	@echo "  make W64 - To make W64Debug, W64Release, and W64Tcov"
	@echo ""
	@echo "  make AndroidDebug"
	@echo "  make AndroidTcov"
	@echo "  make AndroidReleaes"
	@echo "  make Android - To make AndroidDebug, AndroidRelease, and AndroidTcov"
	@echo ""
	@echo "  make all - To make Linux, W64, and Android"


include $(TOP)/BuildTools/Makefile.bottom

clean ::
	$(RM) -rf Data

LinuxDebugRun : LinuxDebug
	BuildTemp/Linux/Debug/bin/JustInts

RunTests::
	BuildTemp/Linux/Tcov/sbin/JustInts
