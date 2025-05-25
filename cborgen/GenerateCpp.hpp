/**
 * Project: Phoenix
 * Time-stamp: <2025-05-24 19:15:52 doug>
 * 
 * @file GenerateCpp.hpp
 * @author Douglas Mark Royer
 * @date 24-FEB-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#ifndef _RIVEREXPLORER_CBORGEN_GENERATECPP_HPP_
#define _RIVEREXPLORER_CBORGEN_GENERATECPP_HPP_

#include <string>
#include "Generate.hpp"

namespace RiverExplorer::cborgen
{
	extern std::string Namespace;
	extern std::string CppNamespace;
	
	/**
	 * Convert the CBOR type, to a C++ type.
	 *
	 * @param The CBOR type.
	 *
	 * @return The C++ type.
	 */
	extern std::string ToCppType(const std::string & In);

	/**
	 * Convert an CBOR namespace to a C++ namespace.
	 */
	extern std::string NamespaceToCppNamespace();

	extern void	GenerateSharedHpp(const std::string & InDirectory);

	extern void	PrintCppNamespaceBegin(std::ostream & Stream);

	extern void	PrintCppNamespaceBegin(std::ostream & Stream,
																		 const std::string & NsName);
	
	extern void	PrintCppNamespaceEnd(std::ostream & Stream);

	extern void	PrintCppNamespaceEnd(std::ostream & Stream,
																	 const std::string & NsName);
	
}
#endif // _RIVEREXPLORER_CBORGEN_GENERATECPP_HPP_
