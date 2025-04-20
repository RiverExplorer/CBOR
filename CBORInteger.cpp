/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 16:02:39 doug>
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
	 * Convert 64-bit unsigned integer from host to network byte
	 * order (big-endian)
	 *
	 * @param Host64 The 64-bit unsigned host value.
	 *
	 * @return The value in netwok byte order.
	 */
	uint64_t
	htonll(uint64_t Host64)
	{
		if (IsBigEndian()) {
			return(Host64); // No conversion needed

		} else {
			// Swap bytes: split into two 32-bit parts,
			// swap them, and use htonl for each
			//
			uint32_t High = (uint32_t)(Host64 >> 32);
			uint32_t Low = (uint32_t)(Host64 & 0xFFFFFFFF);
		
			return ((uint64_t)htonl(Low) << 32) | (uint64_t)htonl(High);
		}
	}

	/**
	 * Convert 64-bit unsigned integer from network to host byte order.
	 *
	 * @param Net64 The network byte order value.
	 *
	 * @return The value in host byte order.
	 */
	inline uint64_t
	ntohll(uint64_t Net64)
	{
		// ntohll is identical to htonll in terms of operation
		//
		return htonll(Net64);
	}

	// Write a unsigned integer in the most
	// compat CBOR format.
	//
	// NOTE:
	// The 32, 16, and 8 bit values call this method
	// because this method always looks for the smallest
	// way to send an unsigned integer.
	//
	ODataStream &
	ODataStream::operator<<(uint64_t Value)
	{
		if (Value < 24) {
			uint8_t Value8 = (uint8_t)UInt_t | (uint8_t)Value;

			_Stream.write(reinterpret_cast<const char*>(&Value8),
										sizeof(Value8));

		} else if (Value <= 0xff) {
			uint8_t Header = ((uint8_t)UInt_t) | ((uint8_t)V8_t);
			uint8_t Value8 = (uint8_t)Value;

			_Stream.write(reinterpret_cast<const char*>(&Header),
										sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&Value8),
										sizeof(Value8));

		} else if (Value <= 0xffff) {
			uint8_t Header = ((uint8_t)UInt_t) | ((uint8_t)V16_t);
			uint16_t Value16 = htons((uint16_t)Value);
			
			_Stream.write(reinterpret_cast<const char*>(&Header),
										sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&Value16),
										sizeof(Value16));

		} else if (Value <= 0xffffffff) {
			uint8_t Header = ((uint8_t)UInt_t) | ((uint8_t)V32_t);
			uint32_t Value32 = htonl((uint32_t)Value);

			_Stream.write(reinterpret_cast<const char*>(&Header),
										sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&Value32),
										sizeof(Value32));

		} else {
			uint8_t Header = ((uint8_t)UInt_t) | ((uint8_t)V64_t);
			uint64_t Value64 = htonll(Value);

			_Stream.write(reinterpret_cast<const char*>(&Header),
										sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&Value64),
										sizeof(Value64));
		}
	
		return(*this);
	}

	// Write a signed integer in the most
	// compat CBOR format.
	//
	// NOTE:
	// The 32, 16, and 8 bit values call this method
	// because this method always looks for the smallest
	// way to send an signed integer.
	//
	ODataStream &
	ODataStream::operator<<(int64_t Value)
	{
		// If positive value, write a positive value.
		//
		if (Value >= 0) {
			_Stream.operator<<((uint64_t)Value);

		} else {
			
			if (-Value < 24) {
				uint8_t PValue = SInt_t | (((uint8_t)-Value) - 1);

				_Stream.write(reinterpret_cast<const char*>(&PValue),
											sizeof(PValue));

			} else if (-Value <= 0xff) {
				uint8_t Header = ((uint8_t)SInt_t) | ((uint8_t)V8_t);
				uint8_t PValue = (((uint8_t)-Value) - 1);

				_Stream.write(reinterpret_cast<const char*>(&Header),
											sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&PValue),
											sizeof(PValue));

			} else if (-Value <= 0xffff) {
				uint8_t Header = ((uint8_t)SInt_t) | ((uint8_t)V16_t);
				uint16_t PValue = htons((uint16_t)-Value) - 1;

				_Stream.write(reinterpret_cast<const char*>(&Header),
											sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&PValue),
											sizeof(PValue));

			} else if (-Value <= 0xffffffff) {
				uint8_t Header = ((uint8_t)SInt_t) | ((uint8_t)V32_t);
				uint32_t PValue = htonl((((uint32_t)-Value) - 1));

				_Stream.write(reinterpret_cast<const char*>(&Header),
											sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&PValue),
											sizeof(PValue));

			} else {
				uint8_t Header = ((uint8_t)SInt_t) | ((uint8_t)V64_t);
				uint64_t PValue = htonll(((-Value) - 1));

				_Stream.write(reinterpret_cast<const char*>(&Header),
											sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&PValue),
											sizeof(PValue));
			}
		}

		return(*this);
	}
	
	// Read an unsigned integer into an unsigned 64-bit value.
	//
	// NOTE:
	// The 32, 16, and 8 bit values call this method.
	// They then cast down to their size.
	//
	IDataStream &
	IDataStream::operator>>(uint64_t & Value)
	{
		// Get the first 8 bits - the header.
		//
		uint8_t Header;

		_Stream.read((char*)&Header, sizeof(Header));

		if (Header <= 0x17) {
			Value = Header;;

		} else if (Header == 0x18) {
			// Unsigned intger: 8-bit follows.
			//
			uint8_t Value8;
			
			_Stream.read((char*)&Value8, sizeof(Value8));
			Value = Value8;
			
		} else if (Header == 0x19) {
			// Unsigned intger: 16-bit follows.
			//
			uint16_t Value16;
			
			_Stream.read((char*)&Value16 , sizeof(Value16));
			Value = ntohs(Value16);

		} else if (Header == 0x1a) {
			// Unsigned intger: 32-bit follows.
			//
			uint32_t Value32;
			
			_Stream.read((char*)&Value32, sizeof(Value32));
			Value = ntohl(Value32);

		} else if (Header == 0x1b) {
			// Unsigned intger: 64-bit follows.
			//
			uint64_t Value64;
			
			_Stream.read((char*)&Value64, sizeof(Value64));
			Value = ntohll(Value64);

		} else {
			// This method should not have been called.
			//
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Wanted an integer."
				" Or wrong size. Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
		}
		
		return(*this);
	}

	// Read an integer into a signed 64-bit value.
	//
	// NOTE:
	// The 32, 16, and 8 bit values call this method.
	// They then cast down to their size.
	//
	IDataStream &
	IDataStream::operator>>(int64_t & Value)
	{
		// Peek at the header, if it is a positive
		// value, then that method will handle it.
		//
		uint8_t Header = _Stream.peek();
		uint8_t Major = Header & MajorMask;

		if (Major <= 0x1b) {
			// The value sent is an unsigned integer.
			// Let the unsigned method handle it.
			//
			uint64 Value64 = Value;
			
			_Stream.operator>>(Value64);
			Value = (int64_t)Value64;

		} else if (Major >= 0x20 && Major <= 0x3b) {
			// Is a negative integer...
			//
			// We peeked at the header above, here
			// we pull header the value out of the stream.
			//
			if (Header >= 0x20 && Header <= 0x37) {
				// Signed integer: Included. (-1 ... -24)
				//
				_Stream.read((char*)Header, sizeof(Header));
				Value = -((Header & ~MajorMask) + 1); // Correct?

			} else if (Value == 0x38) {
				// Signed intger: 8-bit follows.
				//
				int8_t Tmp;
			
				_Stream.read((char*)&Tmp, sizeof(Tmp));
				Value = -(Tmp + 1);
			
			} else if (Value == 0x39) {
				// Signed intger: 16-bit follows.
				//
				int16_t Tmp;
			
				_Stream.read((char*)&Tmp, sizeof(Tmp));
				Value = ntohs((int16_t)(-(Tmp + 1)));
			
			} else if (Value == 0x3a) {
				// Signed intger: 32-bit follows.
				//
				int32_t Tmp;
			
				_Stream.read((char*)&Tmp, sizeof(Tmp));
				Value = ntohl(-(Tmp + 1));
			
			} else if (Value == 0x3b) {
				// Signed integer: 64-bit follows.
				//
				int64_t Tmp;
			
				_Stream.read((char*)&Tmp, sizeof(Tmp));
				Value = ntohll(-(Tmp + 1));
			}
			
		} else {
			// This method should not have been called.
			//
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a int64_t."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
		}
		
		return(*this);
	}

	// Read a 32 bit unsigned integer.
	//
	IDataStream &
	IDataStream::operator>>(uint32_t & Value)
	{
		// Get the first 8 bits - the header.
		//
		uint8_t Header = _Stream.peek();
		
		if (Header <= 0x1a) {

			// Call the 64-bit code, it will get the up to 64-bit value.
			//
			uint64_t Value64;

			_Stream.operator>>(Value64);

			// We know it fits, because of the Header.
			//
			Value = (uint32_t)Value64;

		} else {
			// This method should not have been called.
			//
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a uint32_t."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
		}
		
		return(*this);
	}
	
	// Read a 32 bit signed integer.
	//
	IDataStream &
	IDataStream::operator>>(int32_t & Value)
	{
		// Get the first 8 bits - the header.
		//
		uint8_t Header = _Stream.peek();
		uint8_t MajorCode = Header & MajorMask;
		
		if (Header <= 0x1a) {
			// An unsigned came in.
			//
			int64_t Value64;
			
			_Stream.operator>>(Value64);
			Value = (int32_t)Value64;
			
		if (Header <= 0x3a) {

			// Call the 64-bit code, it reads the correct
			// number of bits, and set the value.
			//
			int64_t Value64;

			_Stream.operator>>(Value64);
			Value = (int32_t)Value64;

		} else {
			// This method should not have been called.
			//
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a int32_t."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
		}
		
		return(*this);
	}
	
	// Read a 16 bit unsigned integer.
	//
	IDataStream &
	IDataStream::operator>>(uint16_t & Value)
	{
		// Get the first 8 bits - the header.
		//
		uint8_t Header = _Stream.peek();


		if (Header <= 0x19) {

			// Call the 64-bit code, it will get the up to 64-bit value.
			//
			uint64_t Value64;

			_Stream.operatoro>>(Value64);

			// We know it fits, because of the Header.
			//
			Value = (uint16_t)Value;

		} else {
			// This method should not have been called.
			//
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a uint32_t."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
		}
		
		return(*this);
	}
	
	// Read a 16 bit signed integer.
	//
	IDataStream &
	IDataStream::operator>>(int16_t & Value)
	{
		// Get the first 8 bits - the header.
		//
		uint8_t Header = _Stream.peek();

		if (Header <= 0x19) {
			// They want to store it as signed.
			//
			_Stream.operator>>((int64_t&)Value);
			
		} else if (Header >= 0x20 && Header <= 0x3a) {

			// Call the 64-bit code, it will get the up to 64-bit value.
			//
			int64_t Value64;

			_Stream.operator>>(Value64);

			// We know it fits, because of the Header.
			//
			Value = (uint16_t)Value;

		} else {
			// This method should not have been called.
			//
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a int32_t."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
		}
		
		return(*this);
	}
	
}
