/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:49:09 doug>
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
	Enum::PrintCppHeader(ofstream & Stream) const
	{
		vector<Item*>::const_iterator MIt;
		EnumValue * Value;
		
		string I = Indent();
		bool PrintedOne = false;

		PrintCppNamespaceBegin(Stream);
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

		Stream << I << "/**" << endl;
		Stream << I << " * Serialize or deserialize a " << Name << " object." << endl;
		Stream << I << " * " << endl;
		Stream << I << " * @param Cbor An CBOR object, initalzied with" << endl;
		Stream << I << " * CBOR_ENCODE, CBOR_DECODE or CBOR_FREE." << endl;
		Stream << I << " * " << endl;
		Stream << I << " * @param Object The address of a " << Name << " object." << endl;
		Stream << I << " * " << endl;
		Stream << I << " * @return true on no errors." << endl;
		Stream << I << " */" << endl;
		Stream << I << "bool cbor_" << Name << "(CBOR * Cbor, " << Name << " * Object);" << endl;
		Stream << endl;

		PrintCppNamespaceEnd(Stream);
	}

	void
	Enum::PrintCppHeaderCbor(ofstream & Stream) const
	{
		PrintCppNamespaceBegin(Stream);
		std::string I = Indent();
		
		Stream << endl << I << "bool cbor_" << Name
					 << "(CBOR * cbors, " << Name << " * obj);" << endl;
		PrintCppNamespaceEnd(Stream);
		
		Stream << endl << I << "extern \"C\" bool cbor_" << Name
					 << "(CBOR * cbors, " << Name << " * obj) {" << endl
					 << "\treturn(" << CppNamespace << "::"
					 << Name << "(cbors, obj));" << endl
					 << "}" << endl;
		
		return;
	}
	
	void
	Enum::PrintCppCBOR(ofstream & Stream) const
	{
		PrintCppNamespaceBegin(Stream);
		std::string I = Indent();
		std::string I2 = Indent(IndentLevel + 1);

		Stream << endl << I << "bool cbor_" << Name
					 << "(CBOR * cbors, " << Name << " * obj) {" << endl;
		Stream << I2 << "return(cbor_enum(cbors, obj);" << endl;
		Stream << I << "}" << endl;
		PrintCppNamespaceEnd(Stream);

		return;
	}

	void
	Enum::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Enum::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Enum::PrintAbnf(ofstream & Stream) const
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
	Enum::PrintServer(ofstream & /*Stream*/) const
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
	EnumValue::PrintCppHeader(ofstream & Stream) const
	{
		string I2 = Indent(IndentLevel + 1);

		Stream << I2 << Name << " = " << Type;

		return;
	}

	void
	EnumValue::PrintCppHeaderCbor(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	// CBOR
	void
	EnumValue::PrintCppCBOR(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	// STUBS
	//
	void
	EnumValue::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	// XSD
	//
	void
	EnumValue::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	// ABNF
	//
	void
	EnumValue::PrintAbnf(ofstream & Stream) const
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
	EnumValue::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}
