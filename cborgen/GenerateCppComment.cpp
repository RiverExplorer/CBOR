/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:51:07 doug>
 * 
 * @file GenerateCppComment.cpp
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
	extern std::string Indent(int Level);
	
	Comment::~Comment()
	{
		/*EMPTY*/

		return;
	}

	static void
	PrintIt(ofstream & Stream, const string & Str)
	{
		std::string::const_iterator SIt;

		std::string I = Indent();

		Stream << I;
		for (SIt = Str.cbegin(); SIt != Str.cend(); SIt++) {
			if (*SIt == '\t' || *SIt == '\r') {
				continue;
			}
			Stream << *SIt;
			if (*SIt == '\n') {
				Stream << I;
			}
		}
		Stream << std::endl;
	}
	
	void
	Comment::PrintCppHeader(ofstream & Stream) const
	{
		PrintIt(Stream, Name);

		return;
	}

	void
	Comment::PrintCppHeaderCbor(ofstream & Stream) const
	{
		PrintIt(Stream, Name);

		return;
	}
	
	void
	Comment::PrintCppCBOR(ofstream & Stream) const
	{
		PrintIt(Stream, Name);

		return;
	}

	void
	Comment::PrintCppStubs(ofstream & Stream) const
	{
		PrintIt(Stream, Name);

		return;
	}

	void
	Comment::PrintXSD(ofstream & Stream) const
	{
		/**@todo */

		return;
	}
	
	void
	Comment::PrintAbnf(ofstream & Stream) const
	{
		Stream << ";";

		for (char C : Name) {
			Stream << C;
			if (C == '\n') {
				Stream << ";";
			}
		}

		return;
	}
	
	void
	Comment::PrintServer(ofstream & Stream) const
	{
		PrintIt(Stream, Name);

		return;
	}
	
}
