/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 19:51:30 doug>
 *
 * @file CborGenShared.cpp
 * @author Douglas Mark Royer
 * @date 24-FEB-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 *
 * Phoenix is a MIME Transport Protocol (MTP).
 * Named Phoenix because it is a risen version version of the
 * Lemonade project if the late 1990's.
 *
 * Unless otherwise specified, all of this code is original
 * code by the author.
 */
#include "cborgen.hpp"

namespace RiverExplorer::cborgen
{
char CborGenSharedHpp[] = R"_LONGSTR_(
/**
 * Project: Phoenix
 * Time-stamp: <2025-03-25 01:27:17 doug>
 * 
 * @file CborGenShared.hpp
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

#ifndef _RIVEREXPLORER_CBORGEN_SHARED_HPP_
#define _RIVEREXPLORER_CBORGEN_SHARED_HPP_

#include <string>
#include <vector>
#include <map>
#include <rpc/rpc.h>
#ifndef W64
#include <unistd.h>
#endif // W64
#include <memory.h>

namespace RiverExplorer::cborgen
{
	/**
	 * An implementation of an CBOR routine
	 * for std::vector<uint8_t>.
	 *
	 * @param Cbor The initalized CBOR object.
	 *
	 * @param Str The address of the std::string.
	 *
	 * @param[optional] MaxLength The maximum size of the string.
	 * @return true on no errors.
	 */
	bool cbor_StdString(CBOR * Cbor,
		std::string * Str,
		uint32_t MaxLength = ~0);
	
	/**
	 * An implementation of an CBOR routine
	 * for std::vector<uint8_t>.
	 *
	 * @param Cbor The initalized CBOR object.
	 *
	 * @param Obj A pointer to the object.
	 *
	 * @param Proc the cborproc_t routine that can
	 * handle type 'T'.
	 *
	 * @param[optional] MaxLength The maximum size of the string.
	 * @return true on no errors.
	 */
	template <class T>
	bool cbor_VectorOf(CBOR * Cbor,
		std::vector<T> * Obj,
		cborproc_t Proc,
		uint32_t MaxLength = ~0)
	{
		bool Results = false;
	
		if (Obj != nullptr && Proc != nullptr) {
				uint32_t Size = Obj->size();
	
			if (cbor_uint32_t(Cbor, &Size)) {
	
				typename std::vector<T>::iterator it;
				T Item;
				uint32_t Offset = 0;
				for (it = Obj->begin(); it != Obj->end(); it++) {
					Item = *it;
					Results = (*Proc)(Cbor, &Item);
					if (!Results) {
						break;
					}
					if (++Offset > MaxLength) {
						break;
					}
				}
			}
		}
	
		return(Results);
	}
} // End namespace RiverExplorer:cborgen

#endif // _RIVEREXPLORER_CBORGEN_SHARED_HPP_
)_LONGSTR_";

void
GenerateCborGenSharedHpp(std::ofstream & Stream)
{
	Stream << CborGenSharedHpp;

	return;
};

}
