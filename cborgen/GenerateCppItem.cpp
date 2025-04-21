/**
 * Project: Phoenix
 * Time-stamp: <2025-04-20 20:37:55 doug>
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
	void
	Item::PrintCppDeclareVariable(ofstream & Stream) const
	{
		string I = Indent();

		if (IsFixedArray || IsVariableArray) {
			if (Type == "opaque") {
				Stream << I << "std::vector<uint8_t> ";
					
			} else if (Type == "string") {
				Stream << I << "std::string ";

			} else {
				Stream << I << "std::vector<" << ToCppType(Type);
				Stream << "> ";
			}
			Stream << Name << ";";

			if (SizeOrValue != "") {
				Stream << I;
				if (IsFixedArray) {
					Stream << endl << I << "// With a fixed size of: " << SizeOrValue;
				} else if (IsVariableArray) {
					Stream << endl << I << "// With a max size of: " << SizeOrValue;
				}
			} else {
				if (IsVariableArray) {
					Stream << endl << I << "// With no size limit.";
				}
			}
		} else {
			std::string CppType = ToCppType(Type);

			if (IsFixedArray || IsVariableArray) {
				Stream << I << "std::vector<" << CppType;
				Stream << ">";
			} else {
				Stream << I << CppType;
			}
			Stream << " ";
			Stream << Name << ";";
		}

		return;
	}
	
}
