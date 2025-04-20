/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:47:28 doug>
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

	void
	Variable::PrintCppHeader(ofstream & Stream, bool PrintExtern) const
	{
		string I;
		
		if (PrintExtern) {
			PrintCppNamespaceBegin(Stream);
			I = Indent();
		} else {
			I = " ";
		}

		string TypeToUse = Type;

		if (Type == "string") {
			TypeToUse = "std::string";
		}
		
		if (IsFixedArray) {
			if (PrintExtern)  {
				Stream << I << "extern " << TypeToUse;
			} else {
				Stream << I << " " << TypeToUse;
			}

			if (IsPointer) {
				Stream << " * ";

			} else if (IsReference) {
				Stream << " & ";

			} else {
				Stream << " ";
			}
			Stream << Name << "[" << SizeOrValue << "];";
			Stream << " // With a fixed size of: " << SizeOrValue << "." << endl;
			
		} else if (IsVariableArray) {
			if (TypeToUse == "std::string") {
				Stream << I << TypeToUse;
			} else {
				Stream << I << "std::vector<" << TypeToUse;
			}

			if (IsPointer) {
				if (TypeToUse == "std::string") {	
					Stream << " * ";
				} else {
					Stream << "*> ";
				}

			} else if (IsReference) {
				if (TypeToUse == "std::string") {	
					Stream << " & ";
				} else {
					Stream << "&> ";
				}

			} else {
				if (TypeToUse == "std::string") {
					Stream << " ";
				} else {
					Stream << "> ";
				}
			}
			if (SizeOrValue == "") {
				Stream << Name << "; //With no size limit." << endl;
			} else {
				Stream << Name << "; //With a max size of: " << SizeOrValue << "." << endl;
			}
		} else {
			if (PrintExtern)  {
				Stream << I << "extern " << TypeToUse;
			} else {
				Stream << I << " " << TypeToUse;
			}
			if (IsPointer) {
				Stream << " * ";
			} else if (IsReference) {
				Stream << " & ";
			} else {
				Stream << " ";
			}
			Stream << Name << ";" << endl;
		}
		Stream << endl;

		if (PrintExtern) {
			PrintCppNamespaceEnd(Stream);
		}

		return;
	}
	
	void
	Variable::PrintCppHeader(ofstream & Stream) const
	{
		PrintCppHeader(Stream, true);
		return;
	}

	void
	Variable::PrintCppHeaderCbor(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Variable::PrintCppCBOR(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Variable::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Variable::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Variable::PrintAbnf(ofstream & Stream) const
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
	Variable::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
}
