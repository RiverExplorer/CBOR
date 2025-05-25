/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:15:41 doug>
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
	extern void GenerateCborGenSharedHpp(std::ostream & Stream);
	
	extern string Namespace;
	extern string InputNoExtension;
	extern int IndentLevel;
	
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
	PrintCppNamespaceBegin(std::ostream & Stream) 
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
	PrintCppNamespaceEnd(std::ostream & Stream)
	{
		if (Namespace != "") {
			IndentLevel--;
			Stream << "} // End namespace "
						 << CppNamespace << std::endl;
		}

		return;
	}

	
	void
	PrintCppNamespaceBegin(std::ostream & Stream, const std::string & Ns)
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
	PrintCppNamespaceEnd(std::ostream & Stream, const std::string & Ns)
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
