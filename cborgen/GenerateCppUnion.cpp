/**
 * Project: Phoenix
q * Time-stamp: <2025-05-25 16:50:56 doug>
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
		return;
	}

	Union::~Union()
	{

		return;
	}
	
	void
	UnionCase::CopyFrom(Variable *& Var)
	{
		if (Var != nullptr) {
			IsFixedArray = Var->IsFixedArray;
			IsVariableArray = Var->IsVariableArray;
			SizeOrValue = Var->SizeOrValue;
			Type = Var->Type;
			DataType = Var->DataType;
			Name = Var->Name;
			OurScope = Var->OurScope;
		}

		return;
	}
	
	void
	Union::PrintCppHeader(ostream & Stream, bool /*WithExtern*/) const
	{
		PrintCppNamespaceBegin(Stream);
		Stream << endl;
		
		string I = Indent();

		Stream << I << "/**" << endl
					 << I << " * @class " << Name
					 << " \"" << Name << ".hpp\" <";

		if (Namespace != "") {
			Stream << NamespaceToIncludePath() << "/";
		}
		Stream << Name << ".hpp>"
					 << endl
					 << I << " * Generated from: union " << Name << endl
					 << I << " */" << endl
					 << I << "class " << Name << endl
					 << I << "{" << endl
					 << I << "public:" << endl;

		IndentLevel++;
		string I2 = Indent();
			
		Stream << I2 << "/**" << endl
					 << I2 << " * " << Name << " - Default Constructor." << endl
					 << I2 << " */" << endl
					 << I2 << Name << "();" << endl
					 << endl
					 << I2 << "/**" << endl
					 << I2 << " * " << Name << " - Destructor." << endl
					 << I2 << " */" << endl
					 << I2 << "virtual ~" << Name << "();" << endl
					 << endl;

		Stream << I2 << "/**" << endl
					 << I2 << " * @return The current content type." << endl
					 << I2 << " */" << endl
					 << I2 << SwitchType << " Type() const {" << endl
					 << I2 << "\treturn(_" << SwitchVariable << ");" << endl
					 << I2 << "}" << endl
					 << endl;

		vector<Item*>::const_iterator CIt;
		UnionCase * Case;
		
		Stream << I << "private:" << endl
					 << endl;
		
		Stream << I2 << "/**" << endl
					 << I2 << " * " << SwitchVariable
					 << " tells you which union case content to use." << endl
					 << I2 << " */" << endl
					 << I2 << SwitchType << " _" << SwitchVariable << ";" << endl;
		
		Stream << endl
					 << I2 << "union  {" << endl;

		IndentLevel++;
		string I3 = Indent();

		for (CIt = Cases.begin(); CIt != Cases.end(); CIt++) {

			if (*CIt != nullptr) {
				Case = dynamic_cast<UnionCase*>(*CIt);

				if (Case != nullptr) {
					if (Case->Type == "void") {
						Stream << I3 << endl
									 << I3 << "/**" << endl
									 << I3 << " * " << Case->CaseValue << " has void data." << endl
									 << I3 << " */" << endl;

					} else {
						Stream << endl
									 << I3 << "/**" << endl
									 << I3 << " * Use when " << SwitchVariable
									 << " (a " << SwitchType << " type)"
									 << " == "
									 << Case->CaseValue
									 << "." << endl
									 << I3 << " */"
									 << endl;

						// Declare namespace gloabal operators.
						//
						Case->PrintCppDeclareVariable(Stream);
					}
				}
				Stream << endl;
			}
		}

		IndentLevel--;
		Stream << endl << I2 << "} _Value;" << endl
					 << endl;

		Stream << I << "public:" << endl
					 << endl
					 << I2 << "/**" << endl
					 << I2 << " * CBOR Serialize a " << Name << " object." << endl
					 << I2 << " *" << endl
					 << I2 << " * @param Out An initialized std::ostream." << endl
					 << I2 << " *" << endl
					 << I2 << " * @param Obj A " << Name  << " object reference." << endl
					 << I2 << " *" << endl
					 << I2 << " * @return Out." << endl
					 << I2 << " */" << endl
					 << I2 << "friend std::ostream & " << CppNamespace << "::operator<<(std::ostream & Out, const " << Name << " & " << " Obj);"  << endl
					 << endl;
					 
		Stream << I2 << "/**" << endl
					 << I2 << " * Deerialize a CBOR strem into a " << Name << " object." << endl
					 << I2 << " *" << endl
					 << I2 << " * @param In An initialized std::istream." << endl
					 << I2 << " *" << endl
					 << I2 << " * @param Obj A " << Name  << " object reference." << endl
					 << I2 << " *" << endl
					 << I2 << " * @return In." << endl
					 << I2 << " */" << endl
					 << I2 << "friend std::istream & " << CppNamespace << "::operator<<(std::istream & Out, " << Name << " & " << " Obj);"  << endl
					 << endl;

		// Generate helper functions.
		//
		for (CIt = Cases.begin(); CIt != Cases.end(); CIt++) {
			if (*CIt != nullptr) {
				Case = dynamic_cast<UnionCase*>(*CIt);

				if (Case != nullptr) {
					if (Case->Type == "void") {
						Stream << I2 << "/**" << endl
									 << I2 << " * " << Case->CaseValue << " has void data." << endl
									 << I2 << " * and has no Get() or Set() method." << endl
									 << I2 << " */" << endl;
						
					} else {
						if (Case->CaseValue == "default") {
							Stream << I2 << "/**" << endl
										 << I2 << " * Set value, and Type()." << endl
										 << I2 << " * Sets the 'default' case value." << endl
										 << I2 << " *" << endl
										 << I2 << " * @param Value Set to this value." << endl
										 << I2 << " */" << endl
										 << I2 << "void Set_Default("
										 << Case->Type << " & Value);" << endl
										 << endl;

							Stream << I2 << "/**" << endl
										 << I2 << " * Get the value of the default type." << endl
										 << I2 << " *" << endl
										 << I2 << " * @return Value for the default." << endl
										 << I2 << " *" << endl
										 << I2 << " */" << endl
										 << I2 << Case->Type << " & Get_Default() const;" << endl
										 << endl;

						} else {
							Stream << I2 << "/**" << endl
										 << I2 << " * Set value, and Type() to " << Case->CaseValue
										 << "." << endl
										 << I2 << " *" << endl
										 << I2 << " * @param Value Set to this value." << endl
										 << I2 << " */" << endl
										 << I2 << "void Set_" << Case->CaseValue << "("
										 << Case->Type << " & Value);" << endl
										 << endl;

							Stream << I2 << "/**" << endl
										 << I2 << " * Get when Type() == " << Case->CaseValue
										 << "." << endl
										 << I2 << " *" << endl
										 << I2 << " * @return Value for " << Case->CaseValue << "." << endl
										 << I2 << " *" << endl
										 << I2 << " * @note When Type() != " << Case->CaseValue << ", then the results are undefined." << endl
										 << I2 << " */" << endl
										 << I2 << Case->Type << " & Get_" << Case->CaseValue << "() const;" << endl
										 << endl;
						}
					}
				}
				Stream << endl;
			}
		}

		Stream << endl
					 << I << "}; // End class " << Name << endl;

		PrintCppNamespaceEnd(Stream);
		
		return;
	}
	
	void
	Union::PrintCppCBOR(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	Union::PrintCppStubs(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	Union::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Union::PrintAbnf(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Union::PrintServer(ostream & /*Stream*/) const
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
	UnionCase::PrintCppHeader(ostream & /*Stream*/, bool /*WithExtern*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	UnionCase::PrintCppCBOR(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	UnionCase::PrintCppStubs(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	UnionCase::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	UnionCase::PrintAbnf(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	UnionCase::PrintServer(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}
