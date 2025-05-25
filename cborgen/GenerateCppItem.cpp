/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:17:27 doug>
 * 
 * @file GenerateCppItem.cpp
 * @author Douglas Mark Royer
 * @date 10-MAR-2025
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
#include "GenerateCpp.hpp"
#include <unistd.h>
#include <fcntl.h>

using namespace std;

namespace RiverExplorer::cborgen
{

	Item::Item()
	{
		IsFixedArray = false;
		IsVariableArray = false;
		SizeOrValue = "";
		Type = "";
		DataType = UnknownType_t;
		OurScope = UnknownScope_t;

		return;
	}
	
	Item::~Item()
	{
		IsFixedArray = false;
		IsVariableArray = false;
		SizeOrValue = "";
		Type = "";
		DataType = UnknownType_t;
		OurScope = UnknownScope_t;

		return;
	}
	
	void
	Item::PrintCppDeclareVariable(ostream & Stream) const
	{
		string I = Indent();

		if (Type == "opaque") {
			if (IsVariableArray) {
				Stream << I << "std::vector<uint8_t> " << Name << ";";

				if (SizeOrValue == "") {
					Stream << "/* No predefined size. */";
				} else {
					Stream << "/* Up to " << SizeOrValue << " character. */";
				}

			} else if (IsFixedArray) {
				Stream << I << "uint8_t " << Name << "[" << SizeOrValue << "];"
							 << "/* Fixed Size of " << SizeOrValue << " */";

			} else {
				Stream << I << "uint8_t " << Name << ";";
			}

		} else if (Type == "string") {
			Stream << I << "std::string " << Name << ";";
			if (SizeOrValue == "") {
				Stream << "/* No predefined length. */";
			} else {
				Stream << "/* Up to " << SizeOrValue << " character. */";
			}

		} else {
			// Is not opaque or string.
			//
			if (IsVariableArray) {
				Stream << I << "std::vector<" << Type << "> " << Name << ";";
				if (SizeOrValue == "") {
					Stream << "/* No predefined length. */";
				} else {
					Stream << "/* Up to " << SizeOrValue << " items. */";
				}

			} else if (IsFixedArray) {
				Stream << I << Type << "> " << Name << "[" << SizeOrValue << "];"
							 << "/* Fixed Size of " << SizeOrValue << " */";
				
			} else {
				Stream << I << Type << " " << Name << ";";
			}
		}

		return;
	}
	
}
