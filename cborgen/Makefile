#
# Project: Phoenix
# Time-stamp: <2025-04-20 22:45:57 doug>
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
TOP			= ..
LIBTOP			= ..

# What to call this program.
# And what the include/RiverExplorer/$(TARGET) header directory will be.
#
TARGET			= cborgen

NOW = $(shell date +%Y%m%dT%H%M%S)

LINUX_CPPFLAGS	+= \
	-I/usr/include/tirpc \
	-DTARG_BINARY_s='"$(TARGET)"' \
	-DFULLVERSION_s='"$(NOW)"' \
	-DOS_s='"Linux"' \
	-DOSVER_s='"$(shell uname -r)"'

include $(TOP)/BuildTools/Makefile.top

CXX_VERSION = -std=c++20

ANTLR4.g4				= cbor.g4

ANTLR4_DIRECTORY	= .

ANTLR4_GENERATED.cpp = \
	cborBaseListener.cpp \
	cborLexer.cpp \
	cborListener.cpp \
	cborParser.cpp

ANTLR4_GENERATED.h = \
	cborBaseListener.h \
	cborLexer.h \
	cborListener.h \
	cborParser.h

ANTLR4.other = \
	cbor.interp \
	cborLexer.interp \
	cborLexer.tokens \
	cbor.tokens

SOURCES.cpp = \
	cborgen.cpp \
	Generate.cpp \
	GenerateCppComment.cpp \
	GenerateCppConstant.cpp \
	GenerateCpp.cpp \
	GenerateCppEnum.cpp \
	GenerateCppItem.cpp \
	GenerateCppMethod.cpp \
	GenerateCppPassThrough.cpp \
	GenerateCppProcedure.cpp \
	GenerateCppProgram.cpp \
	GenerateCppStruct.cpp \
	GenerateCppTypeDef.cpp \
	GenerateCppUnion.cpp \
	GenerateCppVariable.cpp \
	GenerateCppVersion.cpp \
	GenerateCppVersionMethod.cpp \
	CborGenShared.cpp \
	$(ANTLR4_GENERATED.cpp)

SOURCES.hpp = \
	GenerateCpp.hpp \
	Generate.hpp \
	cborgen.hpp \
	CborGenShared.hpp \
	$(ANTLR4_GENERATED.h)

W64_CPPFLAGS	+=

ANDROID_CPPFLAGS += 

ANDROID_LIBS	+= -L/usr/local/Phoenix/Linux/Debug/lib

CPPFLAGS	+= \
	-I/usr/include/antlr4-runtime \
	-I/usr/include/tirpc \
	-I$(ANTLR4_DIRECTORY)

CXXFLAGS	+= -ggdb -std=c++20
LDFLAGS		+= -ggdb -std=c++20

LIBS		+= -lantlr4-runtime

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
	$(RM) XMLResults.xml
	$(RM) $(GENERATED.cpp) $(GENERATED.h) $(GENERATED.other)
	$(RM) -rf GeneratedFromCBOR/* $(ANTLR4_GENERATED.cpp) $(ANTLR4_GENERATED.h)
	$(RM) *.interp *.tokens
	$(RM) cbor.g4

cbor.g4 : ../InternetDraft/Language-00/ANTLR/*.g4
	echo "// DO NOT EDIT, THIS FILE IS GENERATED" > $@
	echo "grammar cbor;" >> $@
	for f in  ../InternetDraft/Language-00/ANTLR/*.g4 ; do \
		export set F=`basename $${f}` ;\
		echo ""	>> $@; \
		echo "//" >> $@; \
		echo "// $${F}"	>> $@; \
		echo "//" >> $@; \
		cat $${f} >> $@; \
		echo ""	>> $@; \
	done

$(ANTLR4_GENERATED.cpp) $(ANTLR4_GENERATED.h) : cbor.g4
	antlr4 -Dlanguage=Cpp -o $(ANTLR4_DIRECTORY) cbor.g4

$(ANTLR4_DIRECTORY)/%.cpp : %.g4
	antlr4 -Dlanguage=Cpp -o $(ANTLR4_DIRECTORY) cbor.g4

$(ANTLR4_DIRECTORY)/%.h : %.g4
	antlr4 -Dlanguage=Cpp -o $(ANTLR4_DIRECTORY) cbor.g4

$(ANTLR4_DIRECTORY)/%.cpp : %.cpp

$(ANTLR_DIRECTORY) :
	-@mkdir $@

LinuxDebugRun : LinuxDebug
	BuildTemp/Linux/Debug/bin/TestClient1

RunTests::
	BuildTemp/Linux/Tcov/sbin/TestClient1
