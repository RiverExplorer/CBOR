/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:49:28 doug>
 * 
 * @file GenerateCpp.cpp
 * @author Douglas Mark Royer
 * @date 24-FEB-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "GenerateCpp.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>

using namespace std;

namespace RiverExplorer::cborgen
{
	extern void GenerateCborGenSharedHpp(std::ofstream & Stream);
	
	extern string Namespace;
	extern string InputNoExtension;
	extern int IndentLevel;
	
	string
	ToCppType(const string & In)
	{
		string Results;

		struct Conversion
		{
			string In;
			string Cpp;
		};

		static Conversion Table[] = {
			{"short", "int16_t"},
			{"u_short", "uint16_t"},
			{"unsigned short", "uint16_t"},
			{"int", "int32_t"},
			{"u_int", "uint32_t"},
			{"unsigned int", "uint32_t"},
			{"long", "int64_t"},
			{"u_long", "uint64_t"},
			{"unsigned long", "uint64_t"},
			{"hyper", "int64_t"},
			{"unsigned hyper", "uint64_t"},
			{"char", "char"},
			{"u_char", "uint8_t"},
			{"string", "std::string"},
			{"bool_t", "bool"},
			{"opaque", "std::vector<uint8_t>"},
			{"",""}
		};
		
		for (int Offset = 0; Table[Offset].In != ""; Offset++) {
			if (In == Table[Offset].In) {
				Results = Table[Offset].Cpp;
				break;
			}
		}
		if (Results == "") {
			Results = In;
		}

		return(Results);
	}

	string
	NamespaceToIncludePath()
	{
		string Results;

		string::const_iterator SIt;

		for (SIt = Namespace.cbegin(); SIt != Namespace.cend(); SIt++) {
			if (*SIt == ':') {
				Results += '/';
			} else {
				Results += *SIt;
			}
		}
		
		return(Results);
	}

	string
	NamespaceToCppNamespace()
	{
		string Results;

		string::const_iterator SIt;

		for (SIt = Namespace.cbegin(); SIt != Namespace.cend(); SIt++) {
			if (*SIt == ':') {
				Results += "::";
			} else {
				Results += *SIt;
			}
		}
		
		return(Results);
	}

	void
	PrintCppNamespaceBegin(std::ofstream & Stream) 
	{
		if (Namespace != "") {
			Stream << std::endl;
			Stream << "namespace " << CppNamespace
						 << std::endl << "{" << std::endl;
			IndentLevel++;
		}

		return;
	}

	void
	PrintCppNamespaceEnd(std::ofstream & Stream)
	{
		if (Namespace != "") {
			IndentLevel--;
			Stream << "} // End namespace "
						 << CppNamespace << std::endl;
		}

		return;
	}

	
	void
	PrintCppNamespaceBegin(std::ofstream & Stream, const std::string & Ns)
	{
		if (Namespace != "") {
			IndentLevel++;

			std::string I = Indent();
			Stream << I << "namespace " << Ns
						 << std::endl << I << "{" << std::endl;
			IndentLevel++;
		}

		return;
	}

	void
	PrintCppNamespaceEnd(std::ofstream & Stream, const std::string & Ns)
	{
		if (Namespace != "") {
			IndentLevel--;
			Stream << Indent() << "} // End namespace "
						 << Ns << std::endl;
			IndentLevel--;
		}

		return;
	}

	void
	GenerateSharedHpp(const string & IntoDirectory)
	{
		std::string Path = IntoDirectory;

		Path += "/";
		Path += "CborGenShared.hpp";

		ofstream Header(Path);

		Header << "/**" << std::endl;
		GenerateThisFileWasGenerated(" * ", Header);
		Header << " */" << std::endl;
		GenerateCborGenSharedHpp(Header);
		
		Header.close();
		
		return;
	}
	
}
