/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:17:47 doug>
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
	PassThrough::PrintCppHeader(ostream & Stream, bool /*WithExtern*/) const
	{
		Stream << Name << std::endl;

		return;
	}

	void
	PassThrough::PrintCppCBOR(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	PassThrough::PrintCppStubs(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	PassThrough::PrintXSD(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	PassThrough::PrintAbnf(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	PassThrough::PrintServer(ostream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}
