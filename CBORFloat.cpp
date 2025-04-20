/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 09:12:21 doug>
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

/**
 * Detect the byte order.
 */
static inline bool  IsBigEndian(void) {
	union {
		uint32_t i;
		char c[4];
	} test = {0x01020304};

	return (test.c[0] == 1);
}

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

inline int16_t
htons(int16_t Host16)
{
	return(htons((uint16_t)Host16));
}

inline int32_t
htonl(int32_t Host32)
{
	return(htonl((uint32_t)Host32));
}

inline int64_t
htonll(int64_t Host64)
{
	return(htonll((uint64_t)Host64));
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

inline int16_t
ntohs(int16_t Net16)
{
	return(ntohs((uint16_t)Net16));
}

inline int32_t
ntohl(int32_t Net32)
{
	return(ntohl((uint32_t)Net32));
}

inline int64_t
ntohll(int64_t Net64)
{
	return(ntohll((uint64_t)Net64));
}

/**
 * Function to check if system is little-endian.
 *
 * @return true if little endian.
 */
static bool
IsLittleEndian()
{
    uint32_t test = 1;
    return *(uint8_t*)&test == 1;
}

/**
 * Convert std::float16_t to network byte order.
 *
 * @param F16 The std::float16_t value.
 *
 * @param[out] Buffer Where the result is stored.
 *
 * @note
 * Buffer must be at least sizeof(std::float16_t) in size.
 */
void
HalfFloatToNetwork(std::float16_t F16, uint8_t * Buffer)
{

	// Copy the 16-bit float to buffer
	//
	memcpy(Buffer, &F16, 2);

	// If system is little-endian, swap bytes
	//
	if (IsLittleEndian()) {
		std::swap(Buffer[0], Buffer[1]);
	}

	return;
}

/**
 * Convert from network byte order to 16-bit float.
 *
 * @param Buffer The network byte order std::float16_t value.
 *
 * @return The std::float16_t value in host order.
 */
std::float16_t
NetworkToHalfFloat(const uint8_t * Buffer)
{
	std::float16_t F16;
	uint8_t Temp[2];

	// Copy buffer to temp.
	//
	memcpy(Temp, Buffer, 2);

	// If system is little-endian, swap bytes.
	//
	if (IsLittleEndian()) {
		std::swap(Temp[0], Temp[1]);
	}

	// Copy to half_float_t.
	//
	memcpy(&F16, Temp, 2);

	return(F16);
}

/**
 * Convert float to network byte order.
 *
 * @param F The float value.
 *
 * @param[out] Buffer Where net network byte order float will be placed.
 */
void
FloatToNetwork(float F, uint8_t * Buffer)
{
	// Copy float to buffer.
	//
	memcpy(Buffer, &F, 4);

	// If system is little-endian, reverse bytes.
	//
	if (IsLittleEndian()) {
		for (size_t i = 0; i < 2; ++i) {
			std::swap(Buffer[i], Buffer[3 - i]);
		}
	}

	return;
}

/**
 * Convert from network byte order to float.
 *
 * @param Buffer The network byte order flaot value.
 *
 * @return The host order float value.
 */
float
NetworkToFloat(const uint8_t * Buffer)
{
	float F;
	uint8_t Temp[4];

	// Copy buffer to temp.
	//
	memcpy(Temp, Buffer, 4);

	// If system is little-endian, reverse bytes.
	//
	if (IsLittleEndian()) {
		for (size_t i = 0; i < 2; ++i) {
			std::swap(Temp[i], Temp[3 - i]);
		}
	}

	// Copy to float.
	//
	memcpy(&F, Temp, 4);

	return(F);
}

/**
 * Convert double to network byte order.
 *
 * @param D The double value.
 *
 * @param[out] Buffer Where the results will be stored.
 */
void
DoubleToNetwork(double D, uint8_t * Buffer)
{
	// Copy double to buffer.
	//
	memcpy(Buffer, &D, 8);

	// If system is little-endian, reverse bytes.
	//
	if (IsLittleEndian()) {
		for (size_t i = 0; i < 4; ++i) {
			std::swap(Buffer[i], Buffer[7 - i]);
		}
	}
}

/**
 * Convert from network byte order to double
 *
 * @param The network byte order double value.
 *
 * @return the double in host byte order.
 */
double
NetworkToDouble(const uint8_t * Buffer)
{
	double D;
	uint8_t Temp[8];

	// Copy buffer to temp.
	//
	memcpy(Temp, Buffer, 8);

	// If system is little-endian, reverse bytes.
	//
	if (IsLittleEndian()) {
		for (size_t i = 0; i < 4; ++i) {
			std::swap(Temp[i], Temp[7 - i]);
		}
	}

	// Copy to double.
	//
	memcpy(&D, Temp, 8);

	return(D);
}

/**
 * Convert long double into network byte order.
 *
 * @param LD The long double value.
 *
 * @param[out] Where the results will be stored.
 *
 * @param Size set to sizeof(long double).
 */
void
LongDoubleToNetwork(long double LD, uint8_t * Buffer, size_t Size)
{
	// Copy long double to buffer.
	//
	memcpy(Buffer, &LD, Size);

	// If system is little-endian, reverse bytes.
	//
	if (IsLittleEndian()) {
		for (size_t i = 0; i < Size / 2; ++i) {
			std::swap(Buffer[i], Buffer[Size - 1 - i]);
		}
	}
}

/**
 * Convert from network byte order to long double.
 *
 * @param Buffer where the network long double is stored.
 *
 * @param Size the sizeof(long double)
 *
 * @return The long double in host byte order.
 */
long double
NetworkToLongDouble(const uint8_t * Buffer, size_t Size)
{
p	long double LD;
	uint8_t Temp[sizeof(long double)];

	// Copy buffer to temp.
	//
	memcpy(Temp, Buffer, Size);

	// If system is little-endian, reverse bytes.
	//
	if (IsLittleEndian()) {
		for (size_t i = 0; i < Size / 2; ++i) {
			std::swap(Temp[i], Temp[Size - 1 - i]);
		}
	}

	// Copy to long double.
	//
	memcpy(&LD, Temp, Size);

	return(LD);
}

namespace RiverExplorer::Phoenix::CBOR
{

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
	ODataStream::operator<<(uint64_t Value)
	{
		if (Value < 24) {
			uint8_t Value8 = UInt_t | (uint8_t)Value;

			_Stream.write(reinterpret_cast<const char*>(&Value8), sizeof(Value8));

		} else if (Value <= 0xff) {
			uint8_t Header = ((uint8_t)UInt_t) | ((uint8_t)V8_t);
			uint8_t Value8 = (uint8_t)Value;

			_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&Value8), sizeof(Value8));

		} else if (Value <= 0xffff) {
			uint8_t Header = ((uint8_t)UInt_t) | ((uint8_t)V16_t);
			uint16_t Value16 = htons((uint16_t)Value);
			
			_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&Value16), sizeof(Value16));

		} else if (Value <= 0xffffffff) {
			uint8_t Header = ((uint8_t)UInt_t) | ((uint8_t)V32_t);
			uint32_t Value32 = htonl((uint32_t)Value);

			_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&Value32), sizeof(Value32));

		} else {
			uint8_t Header = ((uint8_t)UInt_t) | ((uint8_t)V64_t);
			uint64_t Value64 = htonll(Value);

			_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
			_Stream.write(reinterpret_cast<const char*>(&Value64), sizeof(Value64));
		}
	
		return(*this);
	}

	ODataStream &
	operator<<(ODataStream & Out, uint64_t Value)
	{
		return(Out.operator<<(Value));
	}

	ODataStream &
	ODataStream::operator<<(int64_t Value)
	{
		// If positive value, write a positive value.
		//
		if (Value >= 0) {
			_Stream << (uint64_t) Value;

		} else {
			
			if (-Value < 24) {
				uint8_t PValue = SInt_t | (((uint8_t)-Value) - 1);

				_Stream.write(reinterpret_cast<const char*>(&PValue), sizeof(PValue));

			} else if (-Value <= 0xff) {
				uint8_t Header = ((uint8_t)SInt_t) | ((uint8_t)V8_t);
				uint8_t PValue = (((uint8_t)-Value) - 1);

				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&PValue), sizeof(PValue));

			} else if (-Value <= 0xffff) {
				uint8_t Header = ((uint8_t)SInt_t) | ((uint8_t)V16_t);
				uint16_t PValue = htons((uint16_t)-Value) - 1;

				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&PValue), sizeof(PValue));

			} else if (-Value <= 0xffffffff) {
				uint8_t Header = ((uint8_t)SInt_t) | ((uint8_t)V32_t);
				uint32_t PValue = htonl((((uint32_t)-Value) - 1));

				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&PValue), sizeof(PValue));

			} else {
				uint8_t Header = ((uint8_t)SInt_t) | ((uint8_t)V64_t);
				uint64_t PValue = htonll(((-Value) - 1));

				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&PValue), sizeof(PValue));
			}
		}

		return(*this);
	}
	
	ODataStream  &
	operator<<(ODataStream & Out, int64_t Value)
	{
		return(Out.operator<<(Value));
	}
	
	IDataStream &
	IDataStream::operator>>(uint64_t & Value)
	{
		// Get the first 8 bits - the header.
		//
		uint8_t Header;

		_Stream.read((char*)&Header, sizeof(Header));

		if (Header <= 0x17) {
			// Unsigned integer: 0 - 23
			//
			Value = Header & ~MajorMask;

		} else if (Header == 0x18) {
			// Unsigned intger: 8-bit follows.
			//
			uint8_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = Tmp;
			
		} else if (Header == 0x19) {
			// Unsigned intger: 16-bit follows.
			//
			uint16_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = ntohs(Tmp);

		} else if (Header == 0x1a) {
			// Unsigned intger: 32-bit follows.
			//
			uint32_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = ntohl(Tmp);

		} else if (Header == 0x1b) {
			// Unsigned intger: 64-bit follows.
			//
			uint64_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = ntohll(Tmp);

		} else {
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a uint64_t."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
		}
		
		return(*this);
	}
	
	IDataStream &
	operator>>(IDataStream & In, uint64_t & Value)
	{
		return(In.operator>>(Value));
	}

	IDataStream &
	IDataStream::operator>>(int64_t & Value)
	{
		// Get the first 8 bits - the header.
		//
		uint8_t Header;

		_Stream.read((char*)&Header, sizeof(Header));

		if (Header <= 0x17) {
			// Unsigned integer: 0 - 23
			//
			Value = Header & ~MajorMask;

		} else if (Header == 0x18) {
			// Unsigned intger: 8-bit follows.
			//
			int8_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = Tmp;
			
		} else if (Header == 0x19) {
			// Unsigned intger: 16-bit follows.
			//
			int16_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = ntohs(Tmp);

		} else if (Header == 0x1a) {
			// Unsigned intger: 32-bit follows.
			//
			int32_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = ntohl(Tmp);

		} else if (Header == 0x1b) {
			// Unsigned intger: 64-bit follows.
			//
			int64_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = ntohll(Tmp);

		} else if (Value >= 0x20 && Value <= 0x37) {
			// Signed integer: Included. (-1 ... -24)
			//
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
			
		} else {
			// This method should not have been called.
			//
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a uint64_t."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
		}
		
		return(*this);
	}
	
	IDataStream &
	operator>>(IDataStream & In, int64_t & Value)
	{
		return(In.operator>>(Value));
	}

	IDataStream &
	operator>>(IDataStream & In, uint32_t & Value)
	{
		return(In.operator>>(Value));
	}

	IDataStream &
	IDataStream::operator>>(int32_t & Value)
	{
		// Get the first 8 bits - the header.
		//
		uint8_t Header;

		_Stream.read((char*)&Header, sizeof(Header));

		if (Header <= 0x17) {
			// Unsigned integer: 0 - 23
			//
			Value = Header & ~MajorMask;

		} else if (Header == 0x18) {
			// Unsigned intger: 8-bit follows.
			//
			int8_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = Tmp;
			
		} else if (Header == 0x19) {
			// Unsigned intger: 16-bit follows.
			//
			int16_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = ntohs(Tmp);

		} else if (Header == 0x1a) {
			// Unsigned intger: 32-bit follows.
			//
			int32_t Tmp;
			
			_Stream.read((char*)&Tmp, sizeof(Tmp));
			Value = ntohl(Tmp);

		} else if (Header == 0x1b) {
			// Can not put 64-bits into a 32-bit value.
			//
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a int32_t."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
			

		} else if (Value >= 0x20 && Value <= 0x37) {
			// Signed integer: Included. (-1 ... -24)
			//
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
			
		} else {
			// This method should not have been called.
			//
			std::string Err = "Error:"
				" StreamDoesNotMatchType:Provided a uint64_t."
				" Got from stream: ";

			Err += HeaderToString(Header);
			throw new IDataStream::StreamDoesNotMatchType(Err);
		}
		
		return(*this);
	}
	
	IDataStream &
	operator>>(IDataStream & In, int64_t & Value)
	{
		return(In.operator>>(Value));
	}
xx

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
	
	
	ODataStream &
	ODataStream::WriteByteString(uint8_t * ArrayPointer,
															 size_t NumberOfElements)
	{
		if (ArrayPointer != nullptr) {
			uint8_t Header;
			
			if (NumberOfElements < 24) {
				Header = ByteString_t | (uint8_t)NumberOfElements;
				
			} else if (NumberOfElements <= 0xff) {	
				Header = ((uint8_t)ByteString_t) | ((uint8_t)V8_t);
				uint8_t Len = (uint8_t)NumberOfElements;
				
				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len), sizeof(Len));
			
			} else if (NumberOfElements <= 0xffff) {
				Header = ((uint8_t)ByteString_t) | ((uint8_t)V16_t);
				uint16_t Len = (uint16_t)htons((uint16_t)NumberOfElements);
				
				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len), sizeof(Len));
				
			} else if (NumberOfElements <= 0xffffffff) {
				Header = ((uint8_t)ByteString_t) | ((uint8_t)V32_t);
				uint32_t Len = (uint32_t)htonl((uint32_t)NumberOfElements);
				
				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len), sizeof(Len));
				
			} else {
				Header = ((uint8_t)ByteString_t) | ((uint8_t)V64_t);
				uint64_t Len = htonll(NumberOfElements);
				
				_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
				_Stream.write(reinterpret_cast<const char*>(&Len), sizeof(Len));
			}
				
			_Stream.write(reinterpret_cast<const char*>(ArrayPointer),
								NumberOfElements);
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
	
	ODataStream &
	operator<<(ODataStream & Out, std::string & Value)
	{
		return(Out.operator<<(Value));
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
	
	IDataStream &
	operator>>(IDataStream & In, std::string & Value)
	{
		return(In.operator>>(Value));
	}
	
	ODataStream &
	ODataStream::operator<<(std::float16_t Value)
	{	
		uint8_t		Header = ((uint8_t)SimpleOrFloat_t) | ((uint8_t)V16_t);
		uint8_t * Buffer[sizeof(std::float16_t)];

		HalfFloatToNetwork(Value, Buffer);
		_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
		_Stream.write(reinterpret_cast<const char*>(&Buffer), sizeof(std::float16_t));

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
		uint8_t * Buffer[sizeof(float)];

		FloatToNetwork(Value, Buffer);
		_Stream.write(reinterpret_cast<const char*>(&Header), sizeof(Header));
		_Stream.write(reinterpret_cast<const char*>(&Buffer), sizeof(float));

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
