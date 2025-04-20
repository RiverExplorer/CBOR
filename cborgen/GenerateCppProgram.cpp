/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:48:05 doug>
 * 
 * @file GenerateCppProgram.cpp
 * @author Douglas Mark Royer
 * @date 08-MAR-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "cborgen.hpp"

namespace RiverExplorer::cborgen
{
	Program::~Program()
	{
		/*EMPTY*/
		return;
	}
	
	void
	Program::PrintCppHeader(ofstream & Stream) const
	{
		// Name is the 'program' identifier.
		// Type is the 'program' number (value).
		//
		PrintCppNamespaceBegin(Stream);

		std::string I = Indent(IndentLevel + 1);

		Stream << I <<  "/* Begin 'program': " << Name
					 << ", With and ID of: " << Type << " */" << endl;
		Stream << I;
		PrintCppNamespaceBegin(Stream, Name);

		std::vector<Item*>::const_iterator IIt;
		Item * OneItem;
		
		for (IIt = Versions.cbegin(); IIt != Versions.cend(); IIt++) {
			OneItem = *IIt;
			if (OneItem != nullptr) {
				OneItem->PrintCppHeader(Stream);
			}
		}
		
		PrintCppNamespaceEnd(Stream, Name);
		Stream << I <<  "/* End 'program': " << Name
					 << ", With and ID of: " << Type << " */" << endl;
		PrintCppNamespaceEnd(Stream);
		return;
	}

	void
	Program::PrintCppHeaderCbor(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Program::PrintCppCBOR(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	Program::PrintCppStubs(ofstream & Stream) const
	{
		GenerateSharedHpp(CppOutputDirectory);
		
		std::string I = Indent();

		for (Item * AnItem : Versions) {
			AnItem->PrintCppStubs(Stream);
		}
		
		return;
	}

	void
	Program::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Program::PrintAbnf(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Program::PrintServer(ofstream & /*Stream*/) const
	{
	}
	
}
