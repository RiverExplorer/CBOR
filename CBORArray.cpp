/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 18:16:19 doug>
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
	ODataStream::_WriteOpaque(uint8_t * ArrayPointer,
														size_t NumberOfItems,
														size_t SizeOfEachItem)
	{
		uint8_t Header = ByteString_t;
		size_t OctetCount = NumberOfItems * SizeOfEachItem;
		
		if (OctetCount < 24) {
			Header |= (uint8_t)OctetCount;

			_Stream.write(reinterpret_cast<const char*>(&Header),
										sizeof(Header));

		} else if (OctetCount <= 0xff) {	
			Header = ((uint8_t)ByteString_t) | ((uint8_t)V8_t);
				
			_Stream.write(reinterpret_cast<const char*>(&Header),
										sizeof(Header));

			uint8_t Len = (uint8_t)OctetCount;
			_Stream.write(reinterpret_cast<const char*>(&Len),	1);
			
		} else if (OctetCount <= 0xffff) {
			Header = ((uint8_t)ByteString_t) | ((uint8_t)V16_t);
				uint16_t Len = (uint16_t)htons((uint16_t)NumberOfItems);
				
				_Stream.write(reinterpret_cast<const char*>(&Header),
											sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len), sizeof(Len));
				
			} else if (NumberOfItems <= 0xffffffff) {
				Header = ((uint8_t)ByteString_t) | ((uint8_t)V32_t);
				uint32_t Len = (uint32_t)htonl((uint32_t)NumberOfItems);
				
				_Stream.write(reinterpret_cast<const char*>(&Header),
											sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len), sizeof(Len));
				
			} else {
				Header = ((uint8_t)ByteString_t) | ((uint8_t)V64_t);
				uint64_t Len = htonll(NumberOfItems);
				
				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len), sizeof(Len));
			}

		for(size_t i = 0; i < NumberOfItems; i += SizeOfEachItem) {
			_Stream.write(reinterpret_cast<const char*>(&ArrayPointer[i]),
										SizeOfEachItem);
		}

		return(*this);
	}
	
	ODataStream &
	ODataStream::WriteByteString(uint8_t * ArrayPointer,
															 size_t NumberOfItems)
	{
		if (ArrayPointer != nullptr) {
			uint8_t Header;
			
			if (NumberOfItems < 24) {
				Header = (uint8_t)ByteString_t | (uint8_t)NumberOfItems;
				
				_Stream.write(reinterpret_cast<const char*>(&Header),
											sizeof(Header));

			} else if (NumberOfItems <= 0xff) {	
				uint8_t Len = (uint8_t)NumberOfItems;
				Header = ((uint8_t)ByteString_t) | ((uint8_t)V8_t);
				
				_Stream.write(reinterpret_cast<const char*>(&Header),
											sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len),
											sizeof(Len));
			
			} else if (NumberOfItems <= 0xffff) {
				Header = ((uint8_t)ByteString_t) | ((uint8_t)V16_t);
				uint16_t Len = (uint16_t)htons((uint16_t)NumberOfItems);
				
				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len), sizeof(Len));
				
			} else if (NumberOfItems <= 0xffffffff) {
				Header = ((uint8_t)ByteString_t) | ((uint8_t)V32_t);
				uint32_t Len = (uint32_t)htonl((uint32_t)NumberOfItems);
				
				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len), sizeof(Len));
				
			} else {
				Header = ((uint8_t)ByteString_t) | ((uint8_t)V64_t);
				uint64_t Len = htonll(NumberOfItems);
				
				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len), sizeof(Len));
			}
				
			_Stream.write(reinterpret_cast<const char*>(ArrayPointer),
								NumberOfItems);
		} else {
			
			uint8_t Header = ((uint8_t)SimpleOrFloat_t) | ((uint8_t)Null_t);

			_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
		}

		return(*this);
	}

	IDataStream &
	IDataStream::ReadByteString(uint8_t *& ArrayPointer,
															size_t & NumberOfItems)
	{
		uint8_t		Header;
		uint8_t		MajorType;
		uint8_t		Additional;
		
		_Stream.read(reinterpret_cast<char*>(&Header), sizeof(Header));

		MajorType = Header & MajorMask;
		Additional = Header & AdditionalMask;
		
		if (MajorType == ByteString_t) {

			switch (Additional) {

			case V0_t:
				NumberOfItems = Additional;
				break;
				
			case V8_t:
				{
					uint8_t Extra;

					_Stream.read(reinterpret_cast<char*>(&Extra), sizeof(Extra));
					NumberOfItems = Extra;
				}
				break;
				
			case V16_t:
				{
					uint16_t Extra;

					_Stream.read(reinterpret_cast<char*>(&Extra), sizeof(Extra));
					NumberOfItems = ntohs((uint16_t)Extra);
				}
				break;
				
			case V32_t:
				{
					uint32_t Extra;

					_Stream.read(reinterpret_cast<char*>(&Extra), sizeof(Extra));
					NumberOfItems = ntohl((uint32_t)Extra);
				}
				break;
				
			case V64_t:
				{
					uint64_t Extra;

					_Stream.read(reinterpret_cast<char*>(&Extra), sizeof(Extra));
					NumberOfItems = ntohll((uint64_t)Extra);
				}
				break;
				
			}
			ArrayPointer = new uint8_t[NumberOfItems];
			_Stream.read(reinterpret_cast<char*>(ArrayPointer), NumberOfItems);

		} else {
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a ByteString."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
		}

		return(*this);
	}
	
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

}
