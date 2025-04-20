#
# Project: Phoenix
# Time-stamp: <2025-04-17 10:32:24 doug>
#
# Copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
# Author Douglas Mark Royer
# @date 3-JAN-20205
#
# licensed under CC BY 4.0.
#
# RiverExplorer is a trademark of Douglas Mark Royer
#
# Version of this program/library
#
# These two must be set.
# VENDER sets the location name of the include files.
#
VERSION			= 0.0.1
VENDER			= RiverExplorer

# TOP of the build tree where BuildTools/Make... files exist
#
TOP			= .

TESTSUBDIRS		= Tests
SUBDIRS			=
#DOCDIRS		= Documentation

# What to call this program.
# And what the include/$(VENDER)/$(TARGET) header directory will be.
#
TARGET			= CBOR

# Used to determin if we should include files with "" or <>
# When not defined, it uses <>
#
include $(TOP)/BuildTools/Makefile.top.lib

CPPFLAGS		+= -DBUILDING_LIBCBOR

# Version of C++
#
CXX_VERSION = -std=c++23

#
SOURCES.c = 

# Naming convention:
# The .x files create a .cpp and a .hpp .
# Their supporting files are prefixed with Cpp.
#
# Example:
#
# foo.x creates foo.cpp and foo.hpp
#
# So the supporting code goes into CppFoo.cpp
#
# .cpp files that do not have a corrasponding .x file,
# are not prefixed with Cpp.
#
SOURCES.cpp = \
	CBORArray.cpp \
	CBORByteString.cpp \
	CBOR.cpp \
	CBORFloat.cpp \
	CBORInteger.cpp \
	CBORMap.cpp \
	CBORSimpleOrFloat.cpp \
	CBORTag.cpp \
	CBORTextString.cpp

SOURCES.hpp += \
	CBOR.hpp

SOURCES.h		+=

C_OBJECTS         = $(SOURCES.c:.c=.o)
CPP_OBJECTS       = $(SOURCES.cpp:.cpp=.o)

CPPFLAGS					+=

CPPFLAGS.debug		=

CPPFLAGS.release	=

CPPFLAGS.tcov			=

CFLAGS						+=
CXXFLAGS					+=

W64_CPPFLAGS			+= 

ANDROID_CPPFLAGS	+=

LINUX_LIBS				+= -z defs

W64_LIBS					+= -lws2_32

ANDROID_LIBS			+=-z defs

LIBS							+= -ltirpc -lxslt -lxml2 -lssl -lcrypto -lz

include $(TOP)/BuildTools/Makefile.bottom.lib

SubDirs ::
	for D in $(SUBDIRS); do \
		cd $$D ; $(MAKE) $(MAKECMDGOALS); \
	done

clean::
	$(RM) LOG* \#*#
	$(RM) -rf TestCoverage

Test:
	@echo "-----------------------------------"
	@echo "To run the test, type: make RunTests
	@echo "-----------------------------------"

RunTests:: LinuxTcov
	-@for D in $(TESTSUBDIRS); do \
		cd $$D ; $(MAKE) $(MAKECMDGOALS); \
	done
	-@$(RM) -rf TestCoverage
	-@mkdir TestCoverage
	(cd TestCoverage ; gcovr -r .. --html-details TestCoverage.html --decisions --calls)
	@echo "-----------------------------------------------"
	@echo "Test Coverage in TestCoverage/TestCoverage.html"
	@echo "-----------------------------------------------"

gcov : 
	-@$(RM) -rf TestCoverage
	-@mkdir TestCoverage
	(cd TestCoverage ; gcovr -r .. --html-details TestCoverage.html --decisions --calls)

