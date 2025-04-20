/**
 * Project: Phoenix
 * Time-stamp: <2025-04-14 09:50:43 doug>
 *
 * @file Int.cpp
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
namespace RiverExplorer::Phoenix
{

	UnsignedUnsignedInteger64();
		
		/**
		 * UnsignedInteger64 - Constructor.
		 *
		 * @param Value the value.
		 */
		UnsignedInteger64(uint64_t Value);

		/**
		 * UnsignedInteger64 - Destructor.
		 */
		virtual ~UnsignedInteger64();

		/**
		 * Encode to the stream.
		 *
		 * @param Str The DataStream to write to.
		 *
		 * @return true on no errors.
		 * When false, the encoding was not done.
		 */
		virtual bool Encode(DataStream & Str);
		
		/**
		 * Decode from the stream.
		 *
		 * @param Str The DataStream to read from.
		 *
		 * @return true on no errors.
		 * When false, the encoding was not done.
		 */
		virtual bool Decode(DataStream & Str);
		
	};
	
	class SignedInteger64
	{
	public:
		
	};
}
