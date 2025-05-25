/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:17:38 doug>
 * 
 * @file GenerateCppMethod.cpp
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

	Method::Method(Struct & Parent)
		: Parent(Parent)
	{
		/*EMPTY*/
		return;
	}
	
	Method::~Method()
	{
		/*EMPTY*/
		return;
	}
	
	void
	Method::PrintCppHeader(ostream & Stream, bool /*WithExtern*/) const
	{
		std::vector<string>::const_iterator SIt;

		// Get a string set to the current indentation level.
		//
		string I = Indent();

		Stream << I << Type << " " << Name << "(";

		bool PrintedOne = false;
		bool PrintingType = true;

		for (string S : Parameters) {
			if (S == ",") {
				Stream << ", ";
			} else if (S == "*" || S == "&") {
				Stream << " " << S << " ";
			} else {
				if (!PrintedOne) {
					Stream << S;
					PrintedOne = true;
				} else {
					Stream << " " << S;
				}
			}
		}
		Stream << ");" << endl << endl;

		return;
	}

	void
	Method::PrintCppCBOR(ostream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Method::PrintCppStubs(ostream & Stream) const
	{
		std::vector<string>::const_iterator SIt;

		// Get a string set to the current indentation level.
		//
		string I = Indent();
		string I2 = Indent(IndentLevel + 1);
		bool PrintedOne = false;

		Stream << I << Type;
		Stream << " ";

		Stream << Parent.Name << "::" << Name << "(";
		
		for (string S : Parameters) {
			if (S == ",") {
				Stream << ", ";
			} else if (S == "*" || S == "&") {
				Stream << " " << S << " ";
			} else {
				if (!PrintedOne) {
					Stream << S;
					PrintedOne = true;
				} else {
					Stream << " " << S;
				}
			}
		}
		Stream << ")" << endl
					 << I << "{" << endl;
		if (Type != "void") {
			Stream << I2 << "'" << Type << "' Results;" << endl;
		}

		Stream << endl
					 << I2 << "/**@todo write this code */" << endl
					 << endl;
		if (Type != "void") {
			Stream << I2 << "return(Results);" << endl;
		} else {
			Stream << I2 << "return;" << endl;
		}
		Stream << I  << "}" << endl << endl;
	}

	void
	Method::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Method::PrintAbnf(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Method::PrintServer(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
		
}
