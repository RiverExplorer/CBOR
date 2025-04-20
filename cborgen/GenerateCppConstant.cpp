/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:50:58 doug>
 * 
 * @file GenerateCppConstant.cpp
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
	
	Constant::~Constant()
	{
		/*EMPTY*/

		return;
	}

	bool
	IsFloatingPoint(const std::string & Str)
	{
    const std::regex FloatRegex("^-?\\d+(\\.\\d+)?([eE][-+]?\\d+)?$");

    return std::regex_match(Str, FloatRegex);
	}
	
	void
	Constant::PrintCppHeader(ofstream & Stream) const
	{
		string I = Indent(IndentLevel + 1);

		PrintCppNamespaceBegin(Stream);
		// If is string...
		//
		if (Type == "string") {
			size_t Len = SizeOrValue.length();
			
			if ((SizeOrValue[0] == '"' || SizeOrValue[0] == '\'') && Len > 3) { // "x" or 'x' > 3
				if (SizeOrValue[0 ] == '\'') {
					// Use " and not '
					//
					std::string ToUse = SizeOrValue.substr(1, Len - 2);

					Stream << I << "extern const std::string "
								 << Name << "; // = \"" << ToUse << "\";" << endl;
				
				} else {
					Stream << I << "extern const std::string "
								 << Name << "; // = " << SizeOrValue << ";" << endl;
				}
			}
		} else if (Type == "char") {
			size_t Len = SizeOrValue.length();
			
			if (SizeOrValue[0] == '\'' && Len == 3) { // Is char
				Stream << I << "extern const char "
							 << Name << "; // = " << SizeOrValue << ";" << endl;
			} else {
				// What's wrong.
				//
				if (Len > 3) {
					// It is too long, it is a string.
					//
					std::string ToUse = SizeOrValue.substr(1, Len - 2);

					Stream << I << "extern const std::string "
								 << Name << "; // = \"" << ToUse << "\";" << endl;
					
				} else {
					// If is 3 in length, wrong quote.
					//
					Stream << I << "extern const char " << Name
								 << "; // = '" << SizeOrValue[1] << "';" << endl;
				}
			}

		} else if (Type == "quadruple") {
			Stream << I << "extern const long double "
						 << Name << "; // = " << SizeOrValue << ";" << endl;

		} else {
			Stream << I << "extern const " << Type << " " << Name
						 << "; // = " << SizeOrValue << ";" << endl;
		}
		PrintCppNamespaceEnd(Stream);

		return;
	}

	void
	Constant::PrintCppHeaderCbor(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Constant::PrintCppCBOR(ofstream & Stream) const
	{
		string I = Indent(IndentLevel + 1);

		PrintCppNamespaceBegin(Stream);
		// If is string...
		//
		if (Type == "string") {
			size_t Len = SizeOrValue.length();
			
			if ((SizeOrValue[0] == '"' || SizeOrValue[0] == '\'') && Len > 3) { // "x" or 'x' > 3
				if (SizeOrValue[0 ] == '\'') {
					// Use " and not '
					//
					std::string ToUse = SizeOrValue.substr(1, Len - 2);

					Stream << I << "const std::string "
								 << Name << " = \"" << ToUse << "\";" << endl;
				
				} else {
					Stream << I << "const std::string "
								 << Name << " = " << SizeOrValue << ";" << endl;
				}
			}
		} else if (Type == "char") {
			size_t Len = SizeOrValue.length();
			
			if (SizeOrValue[0] == '\'' && Len == 3) { // Is char
				Stream << I << "const char "
							 << Name << " = " << SizeOrValue << ";" << endl;
			} else {
				// What's wrong.
				//
				if (Len > 3) {
					// It is too long, it is a string.
					//
					std::string ToUse = SizeOrValue.substr(1, Len - 2);

					Stream << I << "const std::string "
								 << Name << " = \"" << ToUse << "\";" << endl;
					
				} else {
					// If is 3 in length, wrong quote.
					//
					Stream << I << "const char " << Name
								 << " = '" << SizeOrValue[1] << "';" << endl;
				}
			}

		} else if (Type == "quadruple") {
			Stream << I << "const long double "
						 << Name << " = " << SizeOrValue << ";" << endl;

		} else {
			Stream << I << "const " << Type << " " << Name << " = " << SizeOrValue << ";" << endl;
		}
		PrintCppNamespaceEnd(Stream);

		return;
	}

	void
	Constant::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Constant::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Constant::PrintAbnf(ofstream & Stream) const
	{
		string  ToUse;

		if (SizeOrValue.starts_with("0x")) {
			ToUse = "%x";
			ToUse += SizeOrValue.substr(2);
			
		} else if (SizeOrValue.starts_with("0X")) {
			ToUse = "%x";
			ToUse += SizeOrValue.substr(2);
			
		} else if (SizeOrValue.starts_with("0")) {
			ToUse = "%0";
			ToUse += SizeOrValue.substr(2);
			
		} else if (SizeOrValue.starts_with("0b")) {
			ToUse = "%b";
			ToUse += SizeOrValue.substr(2);
			
		} else if (SizeOrValue.starts_with("0B")) {
			ToUse = "%b";
			ToUse += SizeOrValue.substr(2);
			
		} else {
			if (std::all_of(SizeOrValue.begin(), SizeOrValue.end(), ::isdigit)) {
				ToUse = "%d";
				ToUse += SizeOrValue;

			} else if (SizeOrValue.find('.') != std::string::npos) {
				ToUse = "\"";
				ToUse += SizeOrValue;
				ToUse += "\"";

			} else if (SizeOrValue[0] == '\'') {
				ToUse = '"';
				ToUse += SizeOrValue.substr(1, SizeOrValue.length() - 2);
				ToUse += '"';
				
			} else {
				ToUse = SizeOrValue;
			}
		}
		
		Stream << Name << "\t= " << ToUse << endl;
		
		//Stream << Name << " = " << Type
		//<< "  ; Is a constant with a value of: " << SizeOrValue << endl;
	}
	
	void
	Constant::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
}
