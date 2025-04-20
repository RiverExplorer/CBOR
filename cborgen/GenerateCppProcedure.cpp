/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:48:12 doug>
 * 
 * @file GenerateCppProcedure.cpp
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

	Procedure::~Procedure()
	{
		/*EMPTY*/
		return;
	}
	
	void
	Procedure::PrintCppHeader(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	Procedure::PrintCppHeaderCbor(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Procedure::PrintCppCBOR(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	Procedure::PrintCppStubs(ofstream & Stream) const
	{
		std::string I = Indent();
		std::string I2 = Indent(IndentLevel + 1);
		
		PrintCppDeclareVariable(Stream);

		Stream << " (";
		for (Item * AnItem: Params) {
		}
		Stream << I << ") {" << endl;
		IndentLevel++;
		PrintCppDeclareVariable(Stream);
		
		Stream << I2 << " Results;" << endl << endl;
		Stream << I2 << "/**@todo FILL IN WITH YOUR CODE */" << endl << endl;
		Stream << I2 << "return(Results);" << endl;
		Stream << I << "}" << endl << endl;
		
		return;
	}

	void
	Procedure::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Procedure::PrintAbnf(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Procedure::PrintServer(ofstream & Stream) const
	{
		/**@todo*/
		return;
	}
	
}
