/**
 * Project: Phoenix
 * Time-stamp: <2025-04-17 18:16:37 doug>
 *
 * @file CBOR.hpp
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
#include <iostream>
#include <sstream>
#include <cstdint>
#include <exception>
#include <stdexcept>

#include <string>
#include <list>
#include <vector>
#include <stdfloat>

#ifndef W64
#include <arpa/inet.h>
#else
#endif

namespace RiverExplorer::Phoenix::CBOR
{

	/**
	 * The major types.
	 */
	enum Major_e {

		/**
		 * Unsigned integer.
		 * 0b000_xxxxx (Major Type 0)
		 */
		UInt_t = 0b00000000,

		/**
		 * Signed integer.
		 * 0b001_xxxxx (Major Type 1)
		 */
		SInt_t = 0b00100000,

		/**
		 * Byte string.
		 * 0b010_xxxxx (Major Type 2)
		 */
		ByteString_t			= 0b01000000,

		/**
		 * Text string.
		 * 0b011_xxxxx (Major Type 3)
		 */
		TextString_t			= 0b01100000,

		/**
		 * Array of data items.
		 * 0b100_xxxxx (Major Type 4)
		 */
		Array_t						= 0b10000000,

		/**
		 * Map of pairs of data items.
		 * 0b101_xxxxx (Major Type 5)
		 */
		Map_t							= 0b10100000,

		/**
		 * A tagged data time.
		 * 0b110_xxxxx (Major Type 6)
		 */
		TagOfN_t					= 0b11000000,

		/**
		 * Floating point and simple values.
		 * 0b111_xxxxx (Major Type       7)
		 */
		SimpleOrFloat_t		= 0b11100000
	};

	/**
	 * With Unit_t and SInt_t (Major types 0 and 1).
	 *
	 * @note
	 * 96-bit and 128-bit floats are bigfloats.
	 */
	enum OctetCount_e {
		V0_t	  = 0b00000000, /** Decimal value: 0,  Hex: 0x00*/
		V8_t    = 0b00011000, /** Decimal value: 24, Hex: 0x18 */
		V16_t   = 0b00011001, /** Decimal value: 25, Hex: 0x19 */
		V32_t   = 0b00011010, /** Decimal value: 26, Hex: 0x1a */
		V64_t   = 0b00011011, /** Decimal value: 27, Hex: 0x1b */
		VStop_t = 0x00011111  /** Decimal value: 31, Hex: 0x1f */
	};

	/**
	 * With SimpleOrFloat_t (major type 7).
	 */
	enum SimpleValue_e {
		False_t = 0b00010100, /** Decimal value: 20, Hex: 0x14 */
		True_t  = 0b00010101, /** Decimal value: 21, Hex: 0x15 */
		Null_t  = 0b00010110, /** Decimal value: 22, Hex: 0x16 */
	};
	
	/**
	 * Break stop code.
	 *
	 * 0bxxxx_11111 (31)
	 */
	const uint8_t IndefiniteLength = 0b00011111; 

	/**
	 * The mask to get the major code.
	 */
	const uint8_t MajorMask = 0b11100000;
	
	/**
	 * The mask to get the additional information.
	 */
	const uint8_t AdditionalMask = 0b00011111;
	
	/**
	 * Get the Major Type
	 */
	inline Major_e	Major(uint8_t Header8)
	{
		return((Major_e)(Header8 & MajorMask));
	}

	/**
	 * Set the Major Type
	 *
	 * @param MT The major type.
	 */
	inline uint8_t	Major(Major_e MT)
	{
		return(MT);
	}

#if __BYTE_ORDER == __LITTLE_ENDIAN
	inline bool IsBigEndian() { return(false);}
#elif __BYTE_ORDER == __BIG_ENDIAN
	inline bool IsBigEndian() { return(true);}
#else
	No Clue, now what?
#endif

	/**
	 * Convert a host ordered 64-bit value
	 * to network byte order.
	 *
	 * @param Host64 The host value.
	 *
	 * @return The network byte order value.
	 */
	uint64_t htonll(uint64_t Host64);

	/**
	 * Convert a network ordered 64-bit value
	 * to host byte order.
	 *
	 * @param Net64 The network value.
	 *
	 * @return The host byte order value.
	 */
	inline uint64_t ntohll(uint64_t Net64) {
		return(htonll(Net64));
	}

	/**
	 * Convert host 16-bit float to network byte order.
	 *
	 * @param H16 The host 16-bit value.
	 *
	 * @return The network byte order 16-bit float.
	 */
	inline std::float16_t htonf16(std::float16_t H16) {
		return((std::float16_t)htons(*(uint16_t*)&H16));
	}

	/**
	 * Convert network 16-bit float to host byte order.
	 *
	 * @param N16 The network 16-bit value.
	 *
	 * @return The host byte order 16-bit float.
	 */
	inline std::float16_t ntohf16(std::float16_t N16) {
		return((std::float16_t)htons(*(uint16_t*)&N16));
	}
	
	/**
	 * Convert host 32-bit float to network byte order.
	 *
	 * @param H32 The host 32-bit value.
	 *
	 * @return The network byte order 32-bit float.
	 */
	inline float htonf32(float H32) {
		return((float)htonl(*(uint32_t*)&H32));
	}

	/**
	 * Convert network 32-bit float to host byte order.
	 *
	 * @param N32 The network 32-bit value.
	 *
	 * @return The host byte order 32-bit float.
	 */
	inline float ntohf32(float N32) {
		return((float)htonl(*(uint32_t*)&N32));
	}
	
	/**
	 * Convert host 64-bit float to network byte order.
	 *
	 * @param H64 The host 64-bit value.
	 *
	 * @return The network byte order 64-bit float.
	 */
	inline double htonf64(double H64) {
		return((double)htonll(*(uint64_t*)&H64));
	}

	/**
	 * Convert network 64-bit float to host byte order.
	 *
	 * @param N64 The network 64-bit value.
	 *
	 * @return The host byte order 64-bit float.
	 */
	inline double ntoh64(float N64) {
		return((double)htonll(*(uint64_t*)&N64));
	}
	
	/**
	 * Convert host long double.
	 * Some systems it is 96-bits, other (few) 128-bits.
	 *
	 * @param Hld The host long double value.
	 *
	 * @return The network byte order long double value;
	 */
	long double htonld(long double Fld);

	/**
	 * Convert network long double.
	 * Some systems it is 96-bits, other (few) 128-bits.
	 *
	 * @param Nld The network long double value.
	 *
	 * @return The host byte order long double.
	 */
	inline long double ntohld(long double Nld) {
		return(htonld(Nld));
	}
	
	/**
	 * @class ODataStream "CBOR.hpp" <RiverExplorer/Phoenix/CBOR/CBOR.hpp>
	 */
	class ODataStream
	{
	public:

		/**
		 * ODataStream - Constructor.
		 *
		 * @param Out Where to put the data.
		 */
		ODataStream(std::ostream & Out);

		/**
		 * ODataStream - Destructor.
		 */
		virtual ~ODataStream();

		/**
		 * Write a 64-bit unsigned integer.
		 *
		 * @param Value The value to send.
		 *
		 * @return (*this)
		 */
		ODataStream  & operator<<(uint64_t Value);

		/**
		 * Write a 64-bit signed integer.
		 *
		 * @param Value The value to send.
		 *
		 * @return (*this)
		 */
		ODataStream  & operator<<(int64_t Value);

		/**
		 * Write a 32-bit unsigned integer.
		 * Call the 64-bit version as it always
		 * writes the smallest type that fits the value.
		 *
		 * @param Value The value to send.
		 *
		 * @return (*this)
		 */
		inline ODataStream  & operator<<(uint32_t Value)
		{
			_Stream.operator<<((uint64_t&)Value);
			return(*this);
		}
		
		/**
		 * Write a 32-bit signed integer.
		 * Call the 64-bit version as it always
		 * writes the smallest type that fits the value.
		 *
		 * @param Value The value to send.
		 *
		 * @return (*this)
		 */
		inline ODataStream  & operator<<(int32_t Value)
		{
			_Stream.operator<<((int64_t)Value);
			return(*this);
		}
		
		/**
		 * Write a 16-bit unsigned integer.
		 * Call the 64-bit version as it always
		 * writes the smallest type that fits the value.
		 *
		 * @param Value The value to send.
		 *
		 * @return (*this)
		 */
		ODataStream  & operator<<(uint16_t Value)
		{
			_Stream.operator<<((uint64_t)Value);
			return(*this);
		}
		
		/**
		 * Write a 16-bit signed integer.
		 * Call the 64-bit version as it always
		 * writes the smallest type that fits the value.
		 *
		 * @param Value The value to send.
		 *
		 * @return (*this)
		 */
		ODataStream  & operator<<(int16_t Value)
		{
			_Stream.operator<<((int64_t)Value);
			return(*this);
		}
		
		/**
		 * Write a 8-bit unsigned integer.
		 * Call the 64-bit version as it always
		 * writes the smallest type that fits the value.
		 *
		 * @param Value The value to send.
		 *
		 * @return (*this)
		 */
		ODataStream  & operator<<(uint8_t Value)
		{
			_Stream.operator<<((uint64_t)Value);
			return(*this);
		}
		
		/**
		 * Write a 8-bit signed integer.
		 * Call the 64-bit version as it always
		 * writes the smallest type that fits the value.
		 *
		 * @param Value The value to send.
		 *
		 * @return (*this)
		 */
		ODataStream  & operator<<(int8_t Value)
		{
			_Stream.operator<<((int64_t)Value);
			return(*this);
		}

		/**
		 * Write a std::float16_t value
		 *
		 * @param Value the value to write.
		 *
		 * @return (*this).
		 */
		ODataStream  & operator<<(std::float16_t Value);
		
		/**
		 * Write a float value
		 *
		 * @param Value the value to write.
		 *
		 * @return (*this).
		 */
		ODataStream  & operator<<(float Value);
		
		/**
		 * Write a double value
		 *
		 * @param Value the value to write.
		 *
		 * @return (*this).
		 */
		ODataStream  & operator<<(double Value);
		
		/**
		 * Write a long double value
		 *
		 * @param Value the value to write.
		 *
		 * @return (*this).
		 */
		ODataStream  & operator<<(long double Value);

		/**
		 * Write a boolean value
		 *
		 * @param Value the value to write.
		 *
		 * @return (*this).
		 */
		ODataStream  & operator<<(bool Value);
		
		/**
		 * Write a nullptr value
		 *
		 * @param Value the value to write.
		 *
		 * @return (*this).
		 */
		ODataStream  & operator<<(std::nullptr_t Value);

		/**
		 * Write a text string value
		 *
		 * @param Value the value to write.
		 *
		 * @return (*this).
		 */
		ODataStream  & operator<<(std::string & Value);

		/**
		 * Write a text string value
		 *
		 * @param Value the value to write. The
		 * string must be terminated with a zero (0x00) octet.
		 *
		 * @return (*this).
		 */
		ODataStream  & operator<<(const char * Value);

		/**
		 * Write a fixed size byte array where the size
		 * of each element is sizeof(uint8_t).
		 *
		 * This can be used to send a blob of data,
		 * such as a gif or png file that is already architecture
		 * independently encoded.
		 *
		 * @param ArrayPointer Pointer to the first element in the array.
		 *
		 * @param NumberOfItem The number of items in the array.
		 *
		 * @return The original ODataStream reference.
		 */
		ODataStream & WriteByteString(uint8_t * ArrayPointer,
																	size_t NumberOfItems);

		/**
		 * Write a vector of type 'T'.
		 *
		 * @param VValue a std::vector<T> to send.
		 * Each element of VValue must have a operator<<(T & Value)
		 * method avaiable.
		 *
		 * @param NumberOfItem The number of items in the array.
		 *
		 * @return The original ODataStream reference.
		 */
		template <typename T>
		ODataStream operator<<(const std::vector<T> VValue)
		{
			typename std::vector<T>::const_iterator it;

			for (it = VValue.cbegin(); it != VValue.end; it++) {
				_Stream.operator<<(*it);
			}

			return(*this);
		}

		/**
		 * Write a vector of type 'T' pointer.
		 *
		 * @param VValue a std::vector<T> to send.
		 * And an operator<<(T &* Value) must already be defined.
		 *
		 * When the pointer value is nullptr, then operator<<(nullptr_t)
		 * will be called.
		 *
		 * @param NumberOfItem The number of items in the array.
		 *
		 * @return The original ODataStream reference.
		 */
		template <typename T>
		ODataStream operator<<(const std::vector<T*> VValue)
		{
			typename std::vector<T*>::const_iterator it;

			for (it = VValue.cbegin(); it != VValue.end; it++) {
				if (*it == nullptr) {
					_Stream.operator<<(nullptr);
				} else {
					_Stream.operator<<(**it);
				}
			}

			return(*this);
		}

	private:
		std::ostream	& _Stream;
		
		/**
		 * Write an array of same sized objects.
		 *
		 * @param ArrayPointer A pointer to the first
		 * element in the array.
		 *
		 * @param NumberOfItem The number of items two write.
		 *
		 * @param The size of each element in the array.
		 * It will be one (1), or a multiple of two (2).
		 * (1, 2, 4, 8, ...)
		 *
		 * @return[out] The original IDataStream reference.
		 */
		ODataStream & _WriteArray(uint8_t * ArrayPointer,
															size_t NumberOfItems,
															size_t SizeOfEachItem);
		
		/**
		 * Write an array of same sized objects.
		 * The data will be sent without regard to network byte order.
		 *
		 * @param ArrayPointer A pointer to the first
		 * element in the array.
		 *
		 * @param NumberOfItem The number of items two write.
		 *
		 * @param The size of each element in the array.
		 * It will be one (1), or a multiple of two (2).
		 * (1, 2, 4, 8, ...)
		 *
		 * @return[out] The original IDataStream reference.
		 */
		ODataStream & _WriteOpaque(uint8_t * ArrayPointer,
															 size_t NumberOfItems,
															 size_t SizeOfEachItem);
		
	};

	/**
	 * @class IDataStream "CBOR.hpp" <RiverExplorer/Phoenix/CBOR/CBOR.hpp>
	 */
	class IDataStream
	{
	public:

		/**
		 * IDataStream - Constructor.
		 *
		 * @param In The istream to read from.
		 */
		IDataStream(std::istream & In);

		/**
		 * IDataStream - Destructor.
		 */
		virtual ~IDataStream();
		
		/**
		 * Throws this exception when the data type
		 * received does not match the type requested.
		 *
		 * Example: An integer variable was provided.
		 * Yet a floating value was received.
		 */
		class StreamDoesNotMatchType
			: public std::runtime_error
		{
		public:
			StreamDoesNotMatchType(const std::string & Msg = "")
				: std::runtime_error(Msg)
			{
			}
		};

		/**
		 * Read a an unsigned 64-bit value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(uint64_t & Value);

		/**
		 * Read a an signed 64-bit value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(int64_t & Value);

		/**
		 * Read a an unsigned 32-bit value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(uint32_t & Value);

		/**
		 * Read a an signed 32-bit value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(int32_t & Value);
		
		/**
		 * Read a an unsigned 16-bit value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(uint16_t & Value);

		/**
		 * Read a an signed 16-bit value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(int16_t & Value);
		
		/**
		 * Read a an unsigned 8-bit value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(uint8_t & Value);

		/**
		 * Read a an signed 8-bit value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(int8_t & Value);
		
		/**
		 * Read a an 16-bit float value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(std::float16_t & Value);

		/**
		 * Read a an 32-bit float value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(float & Value);

		/**
		 * Read a an 64-bit float value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(double & Value);

		/**
		 * Read a an 96-bit (or 128-bit) float value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(long double & Value);

		/**
		 * Read a boolean value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(bool & Value);
		
		/**
		 * Read a nullptr value.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(std::nullptr_t & Value);

		/**
		 * Read a text string into memory.
		 *
		 * @param Value Read into this variable.
		 *
		 * @return (*this), the IDataStream.
		 */
		IDataStream  & operator>>(std::string & Value);

		/**
		 * Read a byte array - uint8_t.
		 *
		 * @param ArrayPointer Pointer reference.
		 * The reference will be allocated, or set to nullptr
		 * depending on the input.
		 *
		 * @param NumberOfItem The number of items read.
		 *
		 * @return The original IDataStream reference.
		 */
		IDataStream & ReadByteString(uint8_t *& ArrayPointer,
																 size_t & NumberOfItems);
	
	private:

		std::istream	&	_Stream;

		/**
		 * Read an array of same sized objects.
		 *
		 * @param[out] ArrayPointer Pointer reference.
		 * The reference will be allocated, or set to nullptr
		 * depending on the input.
		 *
		 * @param[out] NumberOfItem The number of items read.
		 *
		 * @param[out] The size of each element in the array.
		 * It will be one (1), or a multiple of two (2).
		 * (1, 2, 4, 8, ...)
		 *
		 * @return[out] The original IDataStream reference.
		 */
		IDataStream & _ReadArray(uint8_t *& ArrayPointer,
														 size_t & NumberOfItems,
														 size_t & SizeOfEachItem);

		/**
		 * Read an array of same sized objects.
		 * The data will be read without regard to network byte order.
		 *
		 * @param[out] ArrayPointer A pointer reference.
		 * The reference will be allocated, or set to nullptr
		 * depending on the input.
		 * Any existing value of ArrayPointer will be set and the
		 * old value overwritten.
		 *
		 * @param[out] NumberOfItem The number of items read.
		 * Any existing value in NumberOfItems will be overwritten.

		 * @param[out] The size of each element in the array.
		 * It will be one (1), or a multiple of two (2).
		 * (1, 2, 4, 8, ...)
		 * Any existing value in SizeOfEachItem will be overwritten.
		 *
		 * @return[out] The original IDataStream reference.
		 */
		IDataStream & _ReadOpaque(uint8_t * ArrayPointer,
														 size_t & NumberOfItems,
														 size_t & SizeOfEachItem);
		
	};

	inline ODataStream &
		operator<<(ODataStream & Out, int64_t Value)
	{
		return(Out.operator<<(Value));
	}
	
	inline IDataStream &
		operator>>(IDataStream & In, int64_t & Value)
	{
		return(In.operator>>(Value));
	}

	inline ODataStream &
		operator<<(ODataStream & Out, uint64_t Value)
	{
		return(Out.operator<<(Value));
	}
		 
	inline IDataStream &
		operator>>(IDataStream & In, uint64_t & Value)
	{
		return(In.operator>>(Value));
	}
		
	inline ODataStream &
		operator<<(ODataStream & Out, float Value)
	{
		return(Out.operator<<(Value));
	}
	
	inline IDataStream &
		operator>>(IDataStream & In, float & Value)
	{
		return(In.operator>>(Value));
	}

	inline ODataStream  &
		operator<<(ODataStream & Out, double Value)
	{
		return(Out.operator<<(Value));
	}
	
	inline IDataStream &
		operator>>(IDataStream & In, double & Value)
	{
		return(In.operator>>(Value));
	}
		
	inline ODataStream &
		operator<<(ODataStream & Out, long double Value)
	{
		return(Out.operator<<(Value));
	}
	
	inline IDataStream &
		operator>>(IDataStream & In, long double & Value)
	{
		return(In.operator>>(Value));
	}

	inline ODataStream &
		operator<<(ODataStream & Out, bool Value)
	{
		return(Out.operator<<(Value));
	}
	
	inline IDataStream &
		operator>>(IDataStream & In, bool & Value)
	{
		return(In.operator>>(Value));
	}

	inline ODataStream &
		operator<<(ODataStream & Out, std::nullptr_t Value)
	{
		return(Out.operator<<(Value));
	}
	
	inline IDataStream &
		operator>>(IDataStream & In, std::nullptr_t & Value)
	{
		return(In.operator>>(Value));
	}

	inline ODataStream &
		operator<<(ODataStream & Out, std::string & Value)
	{
		return(Out.operator<<(Value));
	}
	
	inline IDataStream &
		operator>>(IDataStream & In, std::string & Value)
	{
		return(In.operator>>(Value));
	}

	/**
	 * Print the header type in English for error messages.
	 *
	 * @param Header The header of the CBOR object.
	 *
	 * @return The header, described in English.
	 */
	std::string HeaderToString(uint8_t Header);

}
