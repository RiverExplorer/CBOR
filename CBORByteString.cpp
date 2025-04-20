/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 18:22:17 doug>
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
	
	ODataStream &
	ODataStream::operator<<(std::string & Value)
	{
		const char * Str = Value.c_str();
		size_t Len = Value.length();

		uint8_t Header = TextString_t;

		if (Len < 24) {
			Header |= (uint8_t)Len;
			_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));

		} else if (Len <= 0xff) {
			uint8_t NValue = (uint8_t)Len;
			
			Header |= V8_t;
			_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&NValue), sizeof(NValue));

		} else if (Len <= 0xffff) {
			uint16_t NValue = htons((uint16_t)Len);

			Header |= V16_t;
			_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&NValue), sizeof(NValue));

		} else if (Len <= 0xffffffff) {
			uint32_t NValue = htonl((uint32_t)Len);

			Header |= V32_t;
			_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&NValue), sizeof(NValue));

		} else {
			uint64_t NValue = htonll((uint64_t)Len);
			
			Header |= V64_t;
			_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&NValue), sizeof(NValue));
		}
		_Stream.write(Str, Value.length());

		return(*this);
	}
	
	IDataStream &
	IDataStream::operator>>(std::string & Value)
	{
		uint8_t		Header;
		uint8_t		MajorType;
		uint8_t		Additional;
		size_t		StrLen;
		
		_Stream.read(reinterpret_cast<char*>(&Header), sizeof(Header));

		MajorType = Header & MajorMask;
		Additional = Header & AdditionalMask;
		
		if (MajorType == TextString_t) {

			switch (Additional) {

			case V0_t:
				StrLen = Additional;
				break;
				
			case V8_t:
				{
					uint8_t Extra;

					_Stream.read(reinterpret_cast<char*>(&Extra), sizeof(Extra));
					StrLen = Extra;
				}
				break;
				
			case V16_t:
				{
					uint16_t Extra;

					_Stream.read(reinterpret_cast<char*>(&Extra), sizeof(Extra));
					StrLen = ntohs((uint16_t)Extra);
				}
				break;
				
			case V32_t:
				{
					uint32_t Extra;

					_Stream.read(reinterpret_cast<char*>(&Extra), sizeof(Extra));
					StrLen = ntohl((uint32_t)Extra);
				}
				break;
				
			case V64_t:
				{
					uint64_t Extra;

					_Stream.read(reinterpret_cast<char*>(&Extra), sizeof(Extra));
					StrLen = ntohll((uint64_t)Extra);
				}
				break;
			}
			Value.clear();
			Value.resize(StrLen);
			_Stream.read(&Value[0], StrLen);
			
		} else {
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a TextByte_t."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);

		}
		
		return(*this);
	}
	
	ODataStream &
	ODataStream::operator<<(std::float16_t Value)
	{	
		uint8_t		Header = ((uint8_t)SimpleOrFloat_t) | ((uint8_t)V16_t);

		std::float16_t NValue = htonf16(Value);

		_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
		_Stream.write(reinterpret_cast<const char*>(&NValue),
									sizeof(std::float16_t));

	return(*this);
	}
	
	ODataStream &
	operator<<(ODataStream & Out, std::float16_t Value)
	{
		return(Out.operator<<(Value));
	}
	
	ODataStream &
	ODataStream::operator<<(float Value)
	{
		uint8_t		Header = ((uint8_t)SimpleOrFloat_t) | ((uint8_t)V32_t);

		float NValue = ntohf32(Value);

		_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
		_Stream.write(reinterpret_cast<const char*>(&NValue), sizeof(float));

		return(*this);
	}

	IDataStream &
	operator>>(IDataStream & In, std::float16_t & Value)
	{
	}
	
	ODataStream &
	operator<<(ODataStream & Out, float Value)
	{
		return(Out.operator<<(Value));
	}
	
	IDataStream &
	IDataStream::operator>>(float & Value)
	{
		uint8_t		Header = ((uint8_t)SimpleOrFloat_t) | ((uint8_t)V32_t);
		uint8_t * Buffer[sizeof(float)];

		FloatToNetwork(Value, Buffer);
		_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
		_Stream.write(reinterpret_cast<const char*>(&Buffer), sizeof(float));

		uint8_t		Header;
		uint8_t		Additional;
		
		_Stream.read(reinterpret_cast<char*>(&Header), sizeof(Header));

		if ((Header & MajorMask) == SimpleOrFloat_t) {
			Additional = Header && AdditionalMask;

			if (Additional =
		} else {
			// I should not have been called ...
		}
		
		return(*this);

		return(_Stream, Value);
	}
	
	IDataStream &
	operator>>(IDataStream & In, float Value)
	{
		return(In);
	}

	ODataStream &
	ODataStream::operator<<(double Value)
	{
		uint8_t		Header = ((uint8_t)SimpleOrFloat_t) | ((uint8_t)V64_t);
		uint8_t * Buffer[sizeof(double)];

		DoubleToNetwork(Value, Buffer);
		_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
		_Stream.write(reinterpret_cast<const char*>(&Buffer), sizeof(double));

		return(_Stream, Value);
	}
	
	ODataStream &
	operator<<(ODataStream & Out, double Value)
	{
		return(Out);
	}
	
	IDataStream &
	IDataStream::operator>>(double & Value)
	{
		return(_Stream, Value);
	}
	
	IDataStream &
	operator>>(IDataStream & In, double Value)
	{

		return(In);
	}
		
	ODataStream &
	ODataStream::operator<<(long double Value)
	{
		/**@todo*/
		return(_Stream, Value);
	}
	
	ODataStream &
	operator<<(ODataStream & Out, long double Value)
	{
		/**@todo*/
		return(Out);
	}
	
	IDataStream &
	IDataStream::operator>>(long double & Value)
	{
		/**@todo*/
		return(_Stream, Value);
	}
	
	IDataStream &
	operator>>(IDataStream & In, long double Value)
	{
		/**@todo*/
		return(In);
	}
}
