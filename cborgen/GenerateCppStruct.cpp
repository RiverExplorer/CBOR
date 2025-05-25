/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:19:30 doug>
 * 
 * @file GenerateCppStuct.cpp
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
#include <sys/param.h>

using namespace std;

namespace RiverExplorer::cborgen
{

	extern std::string CppOutputDirectory;
	
	Struct::~Struct()
	{
		/**EMPTY*/
		return;
	}
	
	static void
	PrintSizeInfo(StructMember * Member, std::string & Tabs, ostream & Stream)
	{
		if (Member->IsVariableArray || Member->IsFixedArray) {
			if (Member->SizeOrValue != "") {
				if (Member->IsVariableArray) {
					Stream << endl << Tabs << "// Configured up to "
								 << Member->SizeOrValue << " entries." << endl;
				} else if (Member->IsFixedArray) {
					Stream << endl << Tabs << "// Fixed at "
								 << Member->SizeOrValue << " entries." << endl;
				} else {
					Stream << endl << Tabs << "// No size set for array." << endl;
				}
			}
		}

		return;
	}
	
	void
	Struct::PrintCppHeader(ostream & Stream, bool /*WithExtern*/) const
	{
		Stream << endl;

		string I = Indent();

		PrintCppNamespaceBegin(Stream);
		
		Stream << I << "class " << Name << endl
					 << I << "{" << endl
					 << I << "public:" << endl;
		IndentLevel++;

		string I2 = Indent();
		
		Stream << I2 << "/**" << endl
					 << I2 << " * " << Name << " - Default Constructor." << endl
					 << I2 << " */" << endl
					 << I2 << Name << "();" << endl << endl
					 << I2 << "/**" << endl
					 << I2 << " * " << Name << " - Destructor." << endl
					 << I2 << " */" << endl
					 << I2 << "virtual ~" << Name << "();" << endl
					 << endl
					 << I2 << "/**" << endl
					 << I2 << " * CBOR Serialize a " << Name << " object." << endl
					 << I2 << " *" << endl
					 << I2 << " * @param Out An initialized std::ostream." << endl
					 << I2 << " *" << endl
					 << I2 << " * @param Obj The " << Name << " object to serialize." << endl
					 << I2 << " *" << endl
					 << I2 << " * @return Out" << endl
					 << I2 << " */" << endl
					 << I2 << "friend std::ostream & operator<<(std::ostream & Out, const " << Name << " & Obj);" << endl
					 << endl
					 << I2 << "/**" << endl
					 << I2 << " * Deserialize a CBOR encoded " << Name << " object." << endl
					 << I2 << " *" << endl
					 << I2 << " * @param In An initialized std::istream." << endl
					 << I2 << " *" << endl
					 << I2 << " *" << endl
					 << I2 << " * @param Obj The " << Name << " object to deserialize into." << endl
					 << I2 << " * @return In" << endl
					 << I2 << " */" << endl
					 << I2 << "friend std::istream & operator>>(std::istream & In, " << Name << " & Obj);" << endl
					 << endl;
		
		vector<Item*>::const_iterator MIt;
		StructMember * Member;

		bool NeedSemi;
		
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			NeedSemi = true;

			Member = dynamic_cast<StructMember*>(*MIt);
			if (Member == nullptr) {
				(*MIt)->PrintCppHeader(Stream, false); // Not struct, print it.
				continue;
			}
			
			if (Member->Type == "") {
				continue;
			}
			if (Member->Type == "opaque") {
				Stream << I2 << "std::vector<uint8_t>";
				
			} else if (Member->Type == "string") {
				Stream << I2 << "std::string";
				
			} else {
				PrintSizeInfo(Member, I2, Stream);
				if (Member->IsFixedArray || Member->IsVariableArray) {
					Stream << I2 << "std::vector<" << Member->Type << ">";
				} else {
					Stream << I2 << Member->Type;
				}
			}
			Stream << ' ';
			
			Stream << Member->Name;

			if (Member->IsFixedArray) {
				NeedSemi = false;
				Stream << "; // Fixed Size: " << Member->SizeOrValue << endl;
				
			} else if (Member->IsVariableArray) {
				if (Member->SizeOrValue != "") {
					Stream << ";\t// Max Size: " << Member->SizeOrValue << endl;
					NeedSemi = false;
				}
			}
			if (NeedSemi) {
				Stream << ";" << endl;
			}
			Stream << endl;
		}
		Stream << endl;
		
		IndentLevel--;
		Stream << I << "}; // End of class " << Name << endl << endl
					 << endl;

		PrintCppNamespaceEnd(Stream);
		return;
	}

	void
	Struct::PrintCppCBOR(ostream & Stream) const
	{
		vector<Item*>::const_iterator MIt;
		StructMember * Member;

		bool NeedSemi;
		
		PrintCppNamespaceBegin(Stream);

		Stream << endl;

		string I = Indent();
		string I2;
		string I3;
		
		// Constructor.
		//
		Stream << I << Name << "::" << Name << "()" << endl
					 << I << "{" << endl;
		IndentLevel++;

		I2 = Indent();

		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			NeedSemi = true;
			
			Member = dynamic_cast<StructMember*>(*MIt);
			if (Member == nullptr) {
				(*MIt)->PrintCppCBOR(Stream);
				continue;
			}
			
			if (Member->Type == "") {
				continue;
			}

			if (Member->SizeOrValue != "") {
				PrintSizeInfo(Member, I2, Stream);
				Stream << I2
							 << Member->Name << ".reserve(" << Member->SizeOrValue << ");";
				Stream << endl;
			}
		}
		Stream << endl << I2 << "return;" << endl;
		IndentLevel--;
		Stream << I << "} // End of " << Name << "::" << Name << "()" << endl
					 << endl;
		
		// Destructor.
		//
		Stream << I << Name << "::~" << Name << "()" << endl
					 << I << "{" << endl;
		IndentLevel++;

		I2 = Indent();
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			NeedSemi = true;

			Member = dynamic_cast<StructMember*>(*MIt);
			if (Member == nullptr) {
				//(*Mit)->PrintCppCBOR(Stream);
				continue;
			}
			if (Member->Type == "") {
				continue;
			}

		}
		Stream << I2 << "return;" << endl;
		IndentLevel--;
		Stream << I << "}; // End of " << Name << "::~" << Name << "()" << endl
					 << endl;

		// CBOR
		//
		Stream << I << "bool" << endl
					 << I << Name << "::Cbor(CBOR & Cbor)" << endl
					 << I << "{" << endl;
		IndentLevel++;

		I2 = Indent();
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			NeedSemi = true;

			Member = dynamic_cast<StructMember*>(*MIt);
			if (Member == nullptr) {
				//(*Mit)->PrintCppCBOR(Stream);
				continue;
			}
			if (Member->Type == "") {
				continue;
			}

			if (Member->Type == "opaque") {
				if (Member->SizeOrValue == "") {
					Stream << endl << I2 << "// No size set for array" << endl
								 << I2 << "if (!cbor_VectorOfOpaque(&Cbor, &"
								 << Member->Name << ")) {" << endl
								 << I2 << "\treturn(false);" << endl
								 << I2 << "}" << endl;
				} else {
					PrintSizeInfo(Member, I2, Stream);
					Stream << I2 << "if (!cbor_VectorOfOpaque(&Cbor, &"
								 << Member->Name << ", "
								 << Member->SizeOrValue << ")) {" << endl
								 << I2 << "\treturn(false);" << endl
								 << I2 << "}" << endl;
				}
					
			} else if (Member->Type == "string") {
				if (Member->SizeOrValue == "") {
					Stream << endl << I2 << "// No size set for string." << endl
								 << I2 << "if (!cbor_StdString(&Cbor, &"
								 << Member->Name << ")) {" << endl
								 << I2 << "\treturn(false);" << endl
								 << I2 << "}" << endl;
				} else {
					PrintSizeInfo(Member, I2, Stream);
					Stream << I2 << "if (!cbor_StdString(&Cbor, &"
								 << Member->Name << ", "
								 << Member->SizeOrValue << ")) {" << endl
								 << I2 << "\treturn(false);" << endl
								 << I2 << "}" << endl;
				}

			} else if (IsBuiltInCborType(Member->Type)) {
				if (Member->IsFixedArray || Member->IsVariableArray) {
					if (Member->SizeOrValue == "") {
						Stream << I2 << "if (!cbor_VectorOf<" << Member->Type << ">"
									 << "(&Cbor, &" << Member->Name
									 << ", cbor_" << Member->Type
									 << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					} else {
						PrintSizeInfo(Member, I2, Stream);
						Stream << I2 << "if (!cbor_VectorOf<" << Member->Type  << ">"
									 << "(&Cbor, &" << Member->Name
									 << ", cbor_" << Member->Type
									 << ", " << Member->SizeOrValue
									 << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					}
				} else {
					Stream << I2 << "if (!cbor_" << Member->Type
								 << "(&cbor, &" << Member->Name << ")) {" << endl
								 << I2 << "\treturn(false);" << endl
								 << I2 << "}" << endl;
				}
			} else {
				if (Member->IsFixedArray || Member->IsVariableArray) {
					if (Member->SizeOrValue == "") {
						Stream << I2 << "if (!cbor_VectorOf<" << Member->Type << ">"
									 << "(&Cbor, &" << Member->Name
									 << ", cbor_" << Member->Type
									 << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					} else {
						PrintSizeInfo(Member, I2, Stream);
						Stream << I2 << "if (!cbor_VectorOf<" << Member->Type << ">"
									 << "(&Cbor, &" << Member->Name
									 << ", cbor_" << Member->Type
									 << ", " << Member->SizeOrValue
									 << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					}
				} else {
					Stream << I2
								 << "if (!" << Member->Type << ".Cbor(Cbor)) {" << endl
								 << I2 << "\treturn(false);" << endl
								 << I2 << "}" << endl;
				}
			}
		}
		Stream << I2 << "return;" << endl;
		IndentLevel--;
		Stream << I << "}; // End of " << Name << "::Cbor()" << endl
					 << endl;

		std::string I4 = Indent(IndentLevel + 2);
		I3 = Indent(IndentLevel + 1);
		
		// Now write the cbor_xxx() ...
		//
		Stream << I << "bool cbor_" << Name
					 << "(CBOR * cbors, " << Name << " * objp)" << endl
					 << I << "{" << endl
					 << I2 << "bool Results = false;" << endl << endl
					 << I2 << "if (cbors != nullptr && objp != nullptr) {" << endl
					 << I4 << "Results = objp->Cbor(cbors);" << endl
					 << I3 << "}" << endl
					 << I2 << "return(Results);" << endl
					 << I << "} // End of cbor_" << Name << "()" <<endl;

		PrintCppNamespaceEnd(Stream);

		return;
	}

	void
	Struct::PrintCppStubs(ostream & /*Stream*/) const
	{
		std::vector<Item*>::const_iterator MIt;
		Item * AnItem;
		Method * AMethod;
							
		std::string StubFileName;

		// Sort by method name, so all overloaded methods with
		// the same name, go into the same file.
		//
		std::multimap<std::string,Method*> SameName;
		
		for (Item * MethodItem : Members) {
			AMethod = dynamic_cast<Method*>(MethodItem);
			if (AMethod != nullptr) {
				SameName.insert(std::make_pair(AMethod->Name, AMethod));
			}
		}

		std::string LastName;
		ofstream StubFile;
		bool IsOpen = false;

		for (const auto & Pair : SameName) {
			AMethod = Pair.second;

			if (Pair.first != LastName) {
				if (IsOpen) {
					// End of file.
					//
					if (Namespace != "") {
						IndentLevel--;
						StubFile << "} // End namespace "
									 << NamespaceToCppNamespace()<< std::endl;
					}
					StubFile.close();
				}
				StubFileName = CppOutputDirectory;
				StubFileName += "/";
				StubFileName += InputNoExtension;
				StubFileName += "_";
				StubFileName += AMethod->Parent.Name;
				StubFileName += "_";
				StubFileName += AMethod->Name;
				StubFileName += ".cpp";
				StubFile.open(StubFileName);
				IsOpen = true;

				StubFile << "/**" << std::endl;
				GenerateEditThisFile(" * ", StubFile);
				StubFile << " */" << std::endl << std::endl;
							
				StubFile << "#include \""
							 << InputNoExtension << ".hpp\"" << std::endl;
				
				StubFile << "// Get the CBOR definitions" << std::endl;
				StubFile << "#include <rpc/rpc.h>" << std::endl;
				StubFile << "#include <string>" << std::endl;
				StubFile << "#include <vector>" << std::endl;
				StubFile << std::endl;

				if (Namespace != "") {
					StubFile << std::endl;
					StubFile << "namespace " << NamespaceToCppNamespace()
									 << std::endl << "{" << std::endl;
					IndentLevel++;
				}
			}
			// Print the method.
			//
			AMethod->PrintCppStubs(StubFile);
		}

		return;
	}

	void
	Struct::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	static std::string
	ConvertFromCborComment(Comment * C) 
	{
		std::string Results = ";";
		const char * Str = C->Name.c_str();
		
		if (strncmp(Str, "//", 2) == 0) {
			Results += C->Name.substr(2);
			
		} else if (strncmp(Str, "/*", 2) == 0) {

			// Remove the first two characters "/*" and the last two "*/".
			//
			std::string Tmp = C->Name.substr(2, C->Name.length() - 4);

			// Insert ";" before the first non-whitespace character
			// after each of the line.
			//
			for (int Offset = 2; Offset < C->Name.length() - 4; Offset++) {
				if (C->Name[Offset] == '\r') {
					continue;
				}
				if (C->Name[Offset] == '\n') {
					Results += '\n';
					Offset++;
					
					// Collect up to the first non-whitespace.
					//
					while (C->Name[Offset] == ' ' || C->Name[Offset] == '\t') {
						Results += C->Name[Offset++];
					}
					// Add in "; ".
					//
					Results += "; ";
				} else {
					Results += C->Name[Offset];
				}
			}
		}

		return(Results);
	}
	
	void
	Struct::PrintAbnf(ostream & Stream) const
	{
		vector<Item*>::const_iterator MIt;
		StructMember *	Member;
		size_t					ToPad;
		std::string			Pad;
		std::string			Pad2;
		
		// Get all of the names and find the longest one.
		// We want to align the equal signs.
		//
		size_t Longest = Name.length();
		
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			Member = dynamic_cast<StructMember*>(*MIt);
			if (Member != nullptr) {
				Longest = MAX(Longest, Member->Name.length());
			}
		}
		Stream << endl
					 << Name << " =";

		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			Member = dynamic_cast<StructMember*>(*MIt);

			if (Member != nullptr) {
				Stream << " " << Member->Name;
			}
		}
		Stream << endl << endl;
			
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			Member = dynamic_cast<StructMember*>(*MIt);

			if (Member != nullptr) {

				// This is a hack fix for a bug.
				//
				if (Member->Type == "" && Member->Name == "") {
					continue;
				}
				//
				// End Hack
				
				ToPad = Longest - Member->Name.length();
				Pad.clear();
				Pad.append(ToPad, ' ');

				Pad2.clear();
				Pad2.append(ToPad + 1 + Member->Name.length(), ' ');
				// Any comment about the contets.
				//
				if (Member->IsVariableArray) {
					if (Member->SizeOrValue == "") {
						Stream << Pad2 << "; Unlimited length." << endl;
					} else {
						Stream << Pad2 << "; Up to "
									 << Member->SizeOrValue << " in size."
									 << endl;
					}
				} else if (Member->IsFixedArray) {
						Stream << Pad2 << "; Exactly "
									 << Member->SizeOrValue << " in size."
									 << endl;
				}

				// The variable name.
				//
				Stream << Member->Name << Pad << " = ";
				
				// = The variable type.
				//
				if (Member->Type == "opaque" || Member->Type == "string") {
					// CBOR Opaque is variable or fixed - only.
					//
					if (Member->IsVariableArray) {
						if (Member->SizeOrValue == "") {
							Stream << "NumberOfOctets "
										 << "*uint8_t";
						} else {
							Stream << "NumberOfOctets "
										 << "0*" << Member->SizeOrValue << "uint8_t";
						}
					} else if (Member->IsFixedArray) {
						Stream << Member->SizeOrValue << "uint8_t";
					}

				} else {
					if (Member->IsVariableArray) {
						if (Member->SizeOrValue == "") {
							Stream << "NumberOfOctets "
										 << "*uint8_t";
						} else {
							Stream << "NumberOfOctets "
										 << "0*" << Member->SizeOrValue << Member->Type;
						}
					} else if (Member->IsFixedArray) {
						Stream << Member->SizeOrValue << Member->Type;
					} else {
						Stream << Member->Type;
					}
				}
				Stream << endl << endl;
			} else {
				Comment * C = dynamic_cast<Comment*>(*MIt);

				if (C != nullptr) {
					Stream << ConvertFromCborComment(C);
					Stream << endl;
				}
			}
		}
		Stream << endl << endl;
			
	}
	
	void
	Struct::PrintServer(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	StructMember::~StructMember()
	{
		/*EMPTY*/
		return;
	}
	
	void
	StructMember::PrintCppHeader(ostream & /*Stream*/, bool /*WithExtern*/) const
	{
		/**@todo*/
		return;
	}

	void
	StructMember::PrintCppCBOR (ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	StructMember::PrintCppStubs(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	StructMember::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	StructMember::PrintAbnf(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	StructMember::PrintServer(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}



