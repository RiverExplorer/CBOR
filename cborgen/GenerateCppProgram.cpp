/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:18:07 doug>
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
	Program::PrintCppHeader(ostream & Stream, bool /*WithExtern*/) const
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
	Program::PrintCppCBOR(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	Program::PrintCppStubs(ostream & Stream) const
	{
		GenerateSharedHpp(CppOutputDirectory);
		
		std::string I = Indent();

		for (Item * AnItem : Versions) {
			AnItem->PrintCppStubs(Stream);
		}
		
		return;
	}

	void
	Program::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Program::PrintAbnf(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Program::PrintServer(ostream & /*Stream*/) const
	{
	}
	
}
