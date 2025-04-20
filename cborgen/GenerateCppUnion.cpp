/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:47:45 doug>
 * 
 * @file GenerateCppUnion.cpp
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
#include "GenerateCpp.hpp"

using namespace std;

namespace RiverExplorer::cborgen
{

	Union::Union()
	{
		Default = nullptr;

		return;
	}

	Union::~Union()
	{
		if (Default != nullptr) {
			delete Default;
			Default = nullptr;
		}

		return;
	}
	
	void
	Union::PrintCppHeader(ofstream & Stream) const
	{
		PrintCppNamespaceBegin(Stream);
		Stream << endl;
		
		string I = Indent();

		Stream << I << "/**" << endl;
		Stream << I << " * @class " << Name
					 << " \"" << Name << ".hpp\" <";

		if (Namespace != "") {
			Stream << NamespaceToIncludePath() << "/";
		}
		Stream << Name << ".hpp>"
					 << endl;
		Stream << I << " * Generated from: union " << Name << endl;
		Stream << I << " */" << endl;
		
		Stream << I << "class " << Name << endl;
		Stream << I << "{" << endl;
		Stream << I << "public:" << endl;
		IndentLevel++;

		string I2 = Indent();
		Stream << I2 << Name << "();" << endl;
		Stream << endl;
		Stream << I2 << "virtual ~" << Name << "();" << endl;
		Stream << endl;
		Stream << I2 << "/**" << endl;
		Stream << I2 << " * Serialize or deserialize a " << Name << " object." << endl;
		Stream << I2 << " * " << endl;
		Stream << I2 << " * @param Cbor An CBOR object, initalzied with" << endl;
		Stream << I2 << " * CBOR_ENCODE, CBOR_DECODE or CBOR_FREE." << endl;
		Stream << I2 << " * " << endl;
		Stream << I2 << " * @return true on no errors." << endl;
		Stream << I2 << " */" << endl;
		Stream << I2 << "bool Cbor(CBOR & Cbor);" << endl;
		Stream << endl;

		Stream << I2 << "/**" << endl;
		Stream << I2 << " * " << SwitchVariable
					 << " tells you the which union content to use." << endl;
		Stream << I2 << " */" << endl;
		Stream << I2 << SwitchType << " " << SwitchVariable << ";" << endl;
		
		vector<Item*>::const_iterator CIt;
		UnionCase * Case;

		IndentLevel++;
		string I3 = Indent();

		Stream << endl;
		Stream << I2 << "union {" << endl;
		IndentLevel++;
		string I4 = Indent();
		
		for (CIt = Cases.begin(); CIt != Cases.end(); CIt++) {

			if (*CIt != nullptr) {
				Case = dynamic_cast<UnionCase*>(*CIt);

				if (Case != nullptr) {
					if (Case->Type == "void") {
						Stream << endl << I3
								 << "// " << Case->CaseValue << " has void data." << endl;
					} else {
						Stream << endl;
						Case->PrintCppDeclareVariable(Stream);
						if (Case->CaseValue == "default") {
							Stream << endl << I3
										 << "// This is the DEFAULT variable, use this when"
										 << endl << I3
										 << "// " << SwitchVariable
										 << " (" << SwitchType << ")"
										 << " does not equal any other value."
										 << endl << I3
										 << "// listed here."
										 << endl;
							Stream << endl;
						} else {
							Stream << endl << I3 << "// Use when " << SwitchVariable
										 << " (" << SwitchType << ")"
										 << " == "
										 << Case->CaseValue
										 << "."
										 << endl;
						}
						Stream << endl;
					}
				} else {
					(*CIt)->PrintCppHeader(Stream);
				}
			}
		}
		if (Default != nullptr) {
			Default->PrintCppDeclareVariable(Stream);
			Stream << endl;
		}
		IndentLevel--;
		Stream << I2 << "};" << endl;

		Stream << endl;
		Stream << "}; // End class " << Name << endl;

		PrintCppNamespaceEnd(Stream);
		return;
	}
	
	void
	Union::PrintCppHeaderCbor(ofstream & Stream) const
	{
		PrintCppNamespaceBegin(Stream);
		Stream << "bool cbor_" << Name
					 << "(CBOR * cbors, " << Name << " * obj);" << endl;
		PrintCppNamespaceEnd(Stream);
		Stream << "extern \"C\" bool cbor_" << Name
					 << "(CBOR * cbors, " << Name << " * obj) {" << endl
					 << "\treturn(" << CppNamespace << "::"
					 << Name << "(cbors, obj));" << endl
					 << "}" << endl;
		
		return;
	}
	
	void
	Union::PrintCppCBOR(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	Union::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	Union::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Union::PrintAbnf(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Union::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	UnionCase::~UnionCase()
	{
		/*EMPTY*/
		return;
	}
	
	void
	UnionCase::PrintCppHeader(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	UnionCase::PrintCppHeaderCbor(ofstream & /*Stream*/) const
	{
		/*EMPTY*/
		return;
	}
	
	void
	UnionCase::PrintCppCBOR(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	UnionCase::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	UnionCase::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	UnionCase::PrintAbnf(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	UnionCase::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}
