/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:48:22 doug>
 * 
 * @file GenerateCppPassThrough.cpp
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
	
	PassThrough::~PassThrough()
	{
		/*EMPTY*/
		return;
	}
	
	void
	PassThrough::PrintCppHeader(ofstream & Stream) const
	{
		Stream << Name << std::endl;

		return;
	}

	void
	PassThrough::PrintCppHeaderCbor(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	PassThrough::PrintCppCBOR(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	PassThrough::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	PassThrough::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	PassThrough::PrintAbnf(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	PassThrough::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}
