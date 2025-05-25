/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:17:18 doug>
 * 
 * @file GenerateCppEnum.cpp
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
	
	Enum::~Enum()
	{
		/*EMPTY*/

		return;
	}
	
	void
	Enum::PrintCppHeader(ostream & Stream, bool WithExtern) const
	{
		vector<Item*>::const_iterator MIt;
		EnumValue * Value;
		
		string I = Indent();
		bool PrintedOne = false;

		PrintCppNamespaceBegin(Stream);
		Stream << endl << "#include <limits> " << endl
					 << endl << "#include <iostream> " << endl
					 << endl << "#include <fstream> " << endl
					 << endl << "#include <RiverExplorer/CBOR.hpp> " << endl << endl;

		Stream << I << "enum " << Name << " {" << endl;
		
		for (MIt = Enums.cbegin(); MIt != Enums.cend(); MIt++) {
			Value = dynamic_cast<EnumValue*>(*MIt);
			if (Value == nullptr) {
				Stream << endl;
				(*MIt)->PrintCppHeader(Stream);
				continue;
			}
			if (PrintedOne) {
				Stream << "," << endl;
			}
			Value->PrintCppHeader(Stream);
			PrintedOne = true;
		}

		Stream << endl << I << "};" << endl << endl;

		Stream << I << "/**" << endl
					 << I << " * C++ API: CBOR Serialize a " << Name << " object." << endl
					 << I << " * " << endl
					 << I << " * @param Out A std::ostream" << endl
					 << I << " * " << endl
					 << I << " * @param "<< Name << "Enum The address of a " << Name << " object." << endl
					 << I << " * " << endl
					 << I << " * @return Out." << endl
					 << I << " */" << endl;

		if (WithExtern) {
			Stream << I << "extern ";
		}

		Stream << I << "std::ostream & operator<<(std::ostream & Out, " << Name << " & " << Name  << "Enum);"
					 << endl << endl;
																											
		Stream << I << "/**" << endl
					 << I << " * C++ API: CBOR Deserialize a " << Name << " object." << endl
					 << I << " * " << endl
					 << I << " * @param In A std::istream" << endl
					 << I << " * " << endl
					 << I << " * @param "<< Name << "Enum The address of a " << Name << " object to be filled in from In." << endl
					 << I << " * " << endl
					 << I << " * @return In." << endl
					 << I << " */" << endl;

		if (WithExtern) {
			Stream << I << "extern ";
		}

		Stream << I << "std::ostream & operator>>(std::istream & Out, " << Name << " & " << Name  << "Enum);"
					 << endl << endl;
																											
		Stream << I << "/**" << endl
					 << I << " *" << endl
					 << I << " * C++ API: Convert a " << Name << " enum to C string." << endl
					 << I << " *" << endl
					 << I << " * @param " << Name << "Enum An initialzied " << Name << " enum." << endl
					 << I << " *" << endl
					 << I << " * @return The value of " << Name << "Enum as a string. Will be one of:" << endl
					 << I << " *" << endl
					 << I << " * \t\"ErrorNotInitialized\"" << endl;
		
		std::vector<Item*>::const_iterator SIt;
		Item * OneItem;
		for (SIt = Enums.cbegin(); SIt != Enums.cend(); SIt++) {
			OneItem = *SIt;
			Stream << I << " * \t\"" << OneItem->Name << "\"" << endl;
		}
		
		Stream << I << " */" << endl
					 << I << "extern std::string toString(" << Name << " & " << Name << "Enum);" << endl;

		Stream << endl
					 << I << "/**" << endl
					 << I << " *" << endl
					 << I << " * C++ API: Convert C string back to " << Name << " enum value." << endl
					 << I << " *" << endl
					 << I << " * @param Str A string that is one of the " << Name << " values." << endl
					 << I << " * Where Str must be one of: " << endl
					 << I << " *" << endl;

		for (SIt = Enums.cbegin(); SIt != Enums.cend(); SIt++) {
			OneItem = *SIt;
			Stream << I << " * \t\"" << OneItem->Name << "\"" << endl;
		}
		
		Stream << I << " *" << endl
					 << I << " * @return The value of Object as a " << Name << "." << endl
					 << I << " * When aString is not valid, the return value will be" << endl
					 << I << " * std::numeric_limits<int64_t>::min() cast to a (" << Name << ")." << endl
					 << I << " *" << endl
					 << I << " * @note be sure to: #include <limits>" << endl
					 << I << " */" << endl
					 << I << "extern " << Name << " " << "fromString(std::string & aString);"
					 << endl << endl;

		PrintCppNamespaceEnd(Stream);
	}

	void
	Enum::PrintCppCBOR(ostream & Stream) const
	{
		PrintCppNamespaceBegin(Stream);
		std::string I = Indent();
		std::string I2 = Indent(IndentLevel + 1);

		Stream << "using RiverExplorer;" << endl;
		
		Stream << endl
					 << I << "std::ostream &" << endl
					 << I << "operator<<(std::ostream & Out, " << Name << " & " << Name << "Enum)" << endl
					 << I << "{" << endl
					 << I << "\tint Tmp = " << Name << "Enum;" << endl
					 << I << "\tCBOR::operator<<(Out, Tmp);" << endl
					 << I << endl
					 << I << "\treturn(Out);" << endl
					 << "}" << endl;

		Stream << endl
					 << I << "std::istream &" << endl
					 << I << "operator>>(std::istream & In, " << Name << " & " << Name << "Enum)" << endl
					 << I << "{" << endl
					 << I << "\tint Tmp;" << endl
					 << I << endl
					 << I << "\tCBOR::operator>>(In,Tmp);" << endl
					 << I << "\t" << Name << "Enum = static_cast<" << Name << ">(Tmp);" << endl
					 << I << endl
					 << I << "\treturn(In);" << endl
					 << "}" << endl;

		PrintCppNamespaceEnd(Stream);

		return;
	}

	void
	Enum::PrintCppStubs(ostream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Enum::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Enum::PrintAbnf(ostream & Stream) const
	{
		std::vector<Item*>::const_iterator IIt;
		Item * TheItem;
		EnumValue * Value;
		size_t					ToPad;
		std::string			Pad;
		std::string			Pad2;

		int MaxLength = 70;
		int CurrentLength = 0;
		int RestartLength = 0;

		bool PrintSlash = false;
		
		// Get all of the names and find the longest one.
		// We want to align the equal signs.
		//
		size_t Longest = Name.length();
		
		for (IIt = Enums.cbegin(); IIt != Enums.cend(); IIt++) {
			TheItem = dynamic_cast<EnumValue*>(*IIt);
			if (TheItem != nullptr) {
				Longest = MAX(Longest, TheItem->Name.length());
			}
		}
		Stream << endl;
		ToPad = Longest - Name.length();
		Pad.append(ToPad, ' ');
		Stream << Pad << Name << " =";
		CurrentLength = ToPad + Name.length();
		RestartLength = CurrentLength + 3;

		for (IIt = Enums.cbegin(); IIt != Enums.cend(); IIt++) {
			TheItem = dynamic_cast<EnumValue*>(*IIt);

			if (TheItem == nullptr) {
				(*IIt)->PrintAbnf(Stream);
			}
			if (CurrentLength + TheItem->Name.length() > MaxLength) {
				Pad.clear();
				Pad.append(RestartLength, ' ');
				CurrentLength = ToPad + TheItem->Name.length();
				Stream << endl << Pad;
				if (PrintSlash) {
					CurrentLength += 2;
					Stream << "/ ";
				}
				Stream << TheItem->Name;
					
			} else {
				CurrentLength += TheItem->Name.length();
				if (PrintSlash) {
					CurrentLength += 2;
					Stream << " /";
				}
				Stream << " " << TheItem->Name;
			}
			PrintSlash = true;
		}
		Stream << endl << endl;

		for (IIt = Enums.cbegin(); IIt != Enums.cend(); IIt++) {
			Value = dynamic_cast<EnumValue*>(*IIt);

			if (Value == nullptr) {
				(*IIt)->PrintAbnf(Stream);
			} else {

				ToPad = Longest - Value->Name.length();
				Pad.clear();
				Pad.append(ToPad, ' ');
				Stream << Pad;
				Value->PrintAbnf(Stream);
				Stream << endl << endl;
			}
		}
	}
	
	void
	Enum::PrintServer(ostream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	EnumValue::~EnumValue()
	{
		/*EMPTY*/

		return;
	}

	// HPP
	//
	void
	EnumValue::PrintCppHeader(ostream & Stream, bool /*WithExtern*/) const
	{
		string I2 = Indent(IndentLevel + 1);

		Stream << I2 << Name << " = " << Type;

		return;
	}

	// CBOR
	void
	EnumValue::PrintCppCBOR(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	// STUBS
	//
	void
	EnumValue::PrintCppStubs(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	// XSD
	//
	void
	EnumValue::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	// ABNF
	//
	void
	EnumValue::PrintAbnf(ostream & Stream) const
	{
		Stream << Name << " = ";

		if (Type.starts_with("0x")) {
			std::string Copy = Type;
			
			Stream << Copy.replace(0, 2, "%0");
		} else {
			Stream << Type;
		}
		return;
	}

	// SERVER
	//
	void
	EnumValue::PrintServer(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}
