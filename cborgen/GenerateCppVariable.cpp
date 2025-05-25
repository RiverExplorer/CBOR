/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:20:09 doug>
 * 
 * @file GenerateCppVariable.cpp
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
#include "Generate.hpp"
#include <regex>
#include <cctype>

namespace RiverExplorer::cborgen
{

	using namespace std;
	
	Variable::~Variable()
	{
		/*EMPTY*/
		return;
	}

	// Generate CPP calls this, with extern off.
	//
	void
	Variable::PrintCppHeader(ostream & Stream, bool WithExtern) const
	{
		string I = Indent();
		
		PrintCppNamespaceBegin(Stream);

		string TypeToUse = Type;

		if (Type == "string") {
			TypeToUse = "std::string";
		}
		
		if (IsFixedArray) {
			if (WithExtern)  {
				Stream << I << "extern " << TypeToUse;
			} else {
				Stream << I << TypeToUse;
			}
			Stream << " ";

			Stream << Name << "[" << SizeOrValue << "];"
						 << " // With a fixed size of: " << SizeOrValue << "." << endl;

		} else if (IsVariableArray) {
			if (WithExtern)  {
				Stream << I << "extern ";
			} else {
				Stream << I;
			}
			if (TypeToUse == "std::string") {
				Stream << TypeToUse;
			} else {
				Stream << "std::vector<" << TypeToUse;
			}

			if (TypeToUse == "std::string") {
				Stream << " ";
			} else {
				Stream << "> ";
			}
			if (SizeOrValue == "") {
				Stream << Name << "; // With no size limit." << endl;
			} else {
				Stream << Name << "; // With a max size of: " << SizeOrValue << "." << endl;
			}
		} else {
			if (WithExtern)  {
				Stream << I << "extern " << TypeToUse;
			} else {
				Stream << I << TypeToUse;
			}
			Stream << " " << Name << ";" << endl;
		}
		Stream << endl;

		PrintCppNamespaceEnd(Stream);

		return;
	}
	
	void
	Variable::PrintCppCBOR(ostream & Stream) const
	{
		PrintCppHeader(Stream, false);

		return;
	}

	void
	Variable::PrintCppStubs(ostream & Stream) const
	{
		string I;

		string TypeToUse = Type;

		PrintCppNamespaceBegin(Stream);
		if (Type == "string") {
			TypeToUse = "std::string";
		}
		
		if (IsFixedArray) {
			Stream << I << " " << TypeToUse << " ";

			Stream << Name << "[" << SizeOrValue << "];"
						 << " // With a fixed size of: " << SizeOrValue << "." << endl;
			
		} else if (IsVariableArray) {
			if (TypeToUse == "std::string") {
				Stream << I << TypeToUse;
			} else {
				Stream << I << "std::vector<" << TypeToUse;
			}

			if (TypeToUse == "std::string") {
				Stream << " ";
			} else {
				Stream << "> ";
			}
			if (SizeOrValue == "") {
				Stream << Name << "; // With no size limit." << endl;
			} else {
				Stream << Name << "; // With a max size of: " << SizeOrValue << "." << endl;
			}
		} else {
			Stream << I << " " << TypeToUse << " " << Name << ";" << endl;
		}
		Stream << endl;

		PrintCppNamespaceEnd(Stream);

		return;
	}

	void
	Variable::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Variable::PrintAbnf(ostream & Stream) const
	{
		std::string Pad = std::string(Name.length()  + 3, ' ');

		if (IsVariableArray) {
			Stream << Pad << "; Variable size, up to: " << SizeOrValue << endl;
			
		} else if (IsFixedArray) {
			Stream << Pad << "; Fixed size, of: " << SizeOrValue << endl;
		}
		
		Stream << Name << " = ";

		if (IsVariableArray) {
			Stream << "*" << SizeOrValue;

		} else if (IsFixedArray) {
			Stream << SizeOrValue;
		}
		Stream << Type << endl << endl;
	}
	
	void
	Variable::PrintServer(ostream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
}
