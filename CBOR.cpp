/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 10:28:37 doug>
 *
 * @file Cbor.hpp
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
#include "CBOR.hpp"

#ifndef W64
#include <arpa/inet.h>
#else
what
#endif

#include <bitset>
#include <memory.h>

namespace RiverExplorer::Phoenix::CBOR
{
	/**
	 * Print the header type in English for error messages.
	 *
	 * @param Header The header of the CBOR object.
	 *
	 * @return The header, described in English.
	 */
	static std::string HeaderToString(uint8_t Header)
	{
		std::string Results = "";

		uint8_t Major = Header & MajorMask;
		uint8_t Additional = Header & AdditionalMask;

		switch (Major) {

		case 0:
			Results += "unsigned integer, ";
			break;
			
		case 1:
			Results += "signed integer, ";
			break;
			
		case 2:
			Results += "byte string, ";
			break;
			
		case 3:
			Results += "text string, ";
			break;
			
		case 4:
			Results += "array, ";
			break;
			
		case 5:
			Results += "map, ";
			break;
			
		case 6:
			Results += "Tag of number N, ";
			break;
			
		case 7:
			Results += "simple/float, ";
			break;
		}

		switch (Additional) {

		case V0_t:
			Results += "with value less that 24";
			break;
			
		case V8_t:
			Results += "with 8-bit data that follows.";
			break;
			
		case V16_t:
			Results += "with 16-bit data that follows.";
			break;
			
		case V32_t:
			Results += "with 32-bit data that follows.";
			break;
			
		case V64_t:
			Results += "with 64-bit data that follows.";
			break;

		case 28:
			Results += "Got unknown additional value (28, 0x1c, 0b00011100).";
			break;
			
		case 29:
			Results += "Got unknown additional value (29, 0x1d, 0b00011101).";
			break;

		case 30:
			Results += "Got unknown additional value (30, 0x1e, 0b00011110).";
			break;
			
		case (uint8_t)VStop_t:
			Results = "Got a BREAK stop.";
			break;
		}
		
		return(Results);
	}

	ODataStream::ODataStream(std::ostream & Out)
		: _Stream(Out)
	{
		/*EMPTY*/
		return;
	}

	ODataStream::~ODataStream()
	{
		/*EMPTY*/
		return;
	}

	ODataStream &
	ODataStream::operator<<(std::nullptr_t)
	{
		uint8_t Header = ((uint8_t)SimpleOrFloat_t) | ((uint8_t)Null_t);

		_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));

		return(*this);
	}
	
	ODataStream &
	operator<<(ODataStream & Out, std::nullptr_t Value)
	{
		return(Out.operator<<(Value));
	}
	
	ODataStream &
	ODataStream::operator<<(bool Value)
	{
		uint8_t Header = SimpleOrFloat_t;
		
		if (Value) {
			Header |= True_t;
			
		} else {
			Header |= False_t;
		}
		_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));

		return(*this);
	}
	
	ODataStream &
	operator<<(ODataStream & Out, bool Value)
	{
		return(Out.operator<<(Value));
	}

} // End namespace RiverExplorer::Phoenix::CBOR
