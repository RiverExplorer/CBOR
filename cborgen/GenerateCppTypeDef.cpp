/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:19:43 doug>
 * 
 * @file GenerateCppTypeDef.cpp
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
	using namespace std;
	
	TypeDef::~TypeDef()
	{
		/*EMPTY*/
		return;
	}
	
	void
	TypeDef::PrintCppHeader(ostream & Stream, bool WithExtern) const
	{
		string I = Indent();

		PrintCppNamespaceBegin(Stream);
		Stream << I << "typedef ";
		Declaration->PrintCppHeader(Stream, WithExtern); // ???
		Stream << endl << endl;
		PrintCppNamespaceEnd(Stream);
		
		return;
	}

	void
	TypeDef::PrintCppCBOR(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	TypeDef::PrintCppStubs(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	TypeDef::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	TypeDef::PrintAbnf(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	TypeDef::PrintServer(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}
