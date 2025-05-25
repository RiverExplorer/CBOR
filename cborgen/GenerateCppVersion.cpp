/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:20:27 doug>
 * 
 * @file GenerateCppVersion.cpp
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
	
	Version::Version(Program & ParentProgram)
		: Parent(ParentProgram)
	{
		/*EMPTY*/
		return;
	}
	
	Version::~Version()
	{
		/*EMPTY*/
		return;
	}
	
	void
	Version::PrintCppHeader(ostream & Stream, bool /*WithExtern*/) const
	{
		// Name is the 'version' identifier.
		// Type is the 'version' number (value).
		//
		string I = Indent(IndentLevel + 1);

		Stream << I <<  "/* Begin 'version': " << Name
					 << ", With and ID of: " << Type << " */" << endl;

		Stream << I;
		PrintCppNamespaceBegin(Stream, Name);

		vector<Item*>::const_iterator IIt;
		Item * OneItem;

		IndentLevel++;
		for (IIt = Procedures.cbegin(); IIt != Procedures.cend(); IIt++) {
			OneItem = *IIt;
			if (OneItem != nullptr) {
				OneItem->PrintCppHeader(Stream);
			}
		}
		IndentLevel--;
		
		PrintCppNamespaceEnd(Stream, Name);
		Stream << I <<  "/* End 'version': " << Name
					 << ", With and ID of: " << Type << " */" << endl;

		return;
	}

	void
	Version::PrintCppCBOR(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	Version::PrintCppStubs(ostream & Stream) const
	{
		string I = Indent();
		string StubFileName;
		
		ofstream StubFile;

		for (const Item * AnItem : Procedures) {
			StubFileName = CppOutputDirectory;
			StubFileName += "/";
			StubFileName += InputNoExtension;
			StubFileName += "_";
			StubFileName += Name;
			StubFileName += "_";
			StubFileName += AnItem->Name;
			StubFileName += ".cpp";
			StubFile.open(StubFileName);

			StubFile << "/**" << endl;
				
			GenerateEditThisFile(" * ", StubFile);
			StubFile << " */" << endl << endl << endl;
							
				
			StubFile << "// Get definitions that created us." << endl;
			StubFile << "//" << endl;
			StubFile << "#include \""
							 << InputNoExtension << ".hpp\"" << endl << endl;
				
			StubFile << "// Get the CBOR definitions" << endl;
			StubFile << "//" << endl;

			StubFile << "#include <rpc/rpc.h>" << endl << endl;

			
			StubFile << "#include <string>" << endl;
			StubFile << "#include <vector>" << endl << endl;

			StubFile << "// Template for cbor_ArrayOf<T>() and cbor_StdString()." << endl;
			StubFile << "//" << endl;
				
			StubFile << "#include \"CborGenShared.hpp\"" << endl;
			StubFile << endl;
		
			// Print the method.
			//
			PrintCppNamespaceBegin(StubFile);
			PrintCppNamespaceBegin(StubFile, Parent.Name);

			AnItem->PrintCppStubs(StubFile);

			PrintCppNamespaceEnd(StubFile, Parent.Name);
			PrintCppNamespaceEnd(StubFile);
			StubFile.close();
		}
		IndentLevel--;
		Stream << I << "} // END namespace " << Name << endl;
		
		return;
	}

	void
	Version::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Version::PrintAbnf(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Version::PrintServer(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

}
