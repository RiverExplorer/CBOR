/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:47:56 doug>
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
	TypeDef::PrintCppHeader(ofstream & Stream) const
	{
		string I = Indent();

		PrintCppNamespaceBegin(Stream);
		Stream << I << "typedef ";
		Declaration->PrintCppHeader(Stream, false);
		Stream << endl << endl;
		PrintCppNamespaceEnd(Stream);
		
		return;
	}

	void
	TypeDef::PrintCppHeaderCbor(ofstream & Stream) const
	{
		/*EMPTY*/
		return;
	}
	
	void
	TypeDef::PrintCppCBOR(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	TypeDef::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	TypeDef::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	TypeDef::PrintAbnf(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	TypeDef::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}
