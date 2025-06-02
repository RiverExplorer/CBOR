/**
 * Project: Phoenix
 * Time-stamp: <2025-06-01 22:20:54 doug>
 *
 * @file JustInts.cpp
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
 
#include <fcntl.h>
#include <sys/stat.h>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <stdlib.h>

using namespace RiverExplorer::Phoenix::CBOR;
using namespace std;

class CustomTestListener : public CppUnit::TestListener {
public:
    void startTest(CppUnit::Test *test) override {
        // Print the name of the test when it starts
        std::cout << "Running test: " << test->getName() << std::endl;
    }
};

class JustInts
	: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(JustInts);
	CPPUNIT_TEST(TestConstructor);

	CPPUNIT_TEST(TestUInt8Zero);
	CPPUNIT_TEST(TestUInt8Sequence);
	CPPUNIT_TEST(TestUInt8Random);

	CPPUNIT_TEST(TestInt8Zero);
	CPPUNIT_TEST(TestInt8Sequence);
	CPPUNIT_TEST(TestInt8Random);

	CPPUNIT_TEST(TestUInt16Zero);
	CPPUNIT_TEST(TestUInt16Sequence);
	CPPUNIT_TEST(TestUInt16Random);

	CPPUNIT_TEST(TestInt16Zero);
	CPPUNIT_TEST(TestInt16Sequence);
	CPPUNIT_TEST(TestInt16Random);

	CPPUNIT_TEST(TestUInt32Zero);
	CPPUNIT_TEST(TestUInt32Sequence);
	CPPUNIT_TEST(TestUInt32Random);

	CPPUNIT_TEST(TestInt32Zero);
	CPPUNIT_TEST(TestInt32Sequence);
	CPPUNIT_TEST(TestInt32Random);

	CPPUNIT_TEST(TestUInt64Zero);
	CPPUNIT_TEST(TestUInt64Sequence);
	CPPUNIT_TEST(TestUInt64Random);

	CPPUNIT_TEST(TestInt64Zero);
	CPPUNIT_TEST(TestInt8Sequence);
	CPPUNIT_TEST(TestInt64Random);

	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();

	void TestConstructor();

	void TestUInt8Zero();
	void TestUInt8Sequence();
	void TestUInt8Random();

	void TestInt8Zero();
	void TestInt8Sequence();
	void TestInt8Random();

	void TestUInt16Zero();
	void TestUInt16Sequence();
	void TestUInt16Random();

	void TestInt16Zero();
	void TestInt16Sequence();
	void TestInt16Random();

	void TestUInt32Zero();
	void TestUInt32Sequence();
	void TestUInt32Random();

	void TestInt32Zero();
	void TestInt32Sequence();
	void TestInt32Random();

	void TestUInt64Zero();
	void TestUInt64Sequence();
	void TestUInt64Random();

	void TestInt64Zero();
	void TestInt64Sequence();
	void TestInt64Random();
};

CPPUNIT_TEST_SUITE_REGISTRATION(JustInts);

void
JustInts::setUp()
{
	mkdir("./Data", 0700);

	return;
}

void
JustInts::tearDown()
{

	return;
}

void
JustInts::TestConstructor()
{
	ofstream OutFile("NotUsed");
	OCborStream Out(OutFile);

	ifstream InFile("NotUsed");
	ICborStream In(InFile);

	return;
}

void
JustInts::TestUInt8Zero()
{
	const char * File = "Data/UInt8File0";

	ofstream OutFile(File);
	OCborStream Out(OutFile);
	uint8_t Zero = 0;

	Out << Zero;
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	uint8_t Got;
	
	In >> Got;

	if (Got == Zero) {
		printf("TestInt8:Expected %d, got  %d\n", Zero, Got);
	} else {
		printf("ERROR:TestInt8:Expected %d, got  %d\n", Zero, Got);
	}
	CPPUNIT_ASSERT(Zero == Got);

	InFile.close();

	return;
}

void
JustInts::TestUInt8Sequence()
{
	uint8_t Array[100];

	srand(time(0));
	
	int Offset;

	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = Offset;
	}
	
	const char * File = "Data/UInt8Sequence";
	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	uint8_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestUInt8Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestUInt8Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestUInt8Random()
{
	uint8_t Array[100];

	srand(time(0));
	
	int Offset;

	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = (rand() % 0xff);
	}
	
	const char * File = "Data/UInt8Random";
	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	uint8_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestUInt8Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestUInt8Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestInt8Zero()
{
	const char * File = "Data/Int8File0";

	ofstream OutFile(File);
	OCborStream Out(OutFile);
	int8_t Zero = 0;

	Out << Zero;
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	uint8_t Got;
	
	In >> Got;
	printf("TestInt16:Expected %d, got  %d\n", Zero, Got);
	CPPUNIT_ASSERT(Zero == Got);

	InFile.close();
	return;
}

void
JustInts::TestInt8Sequence()
{
	int8_t Array[200];

	int Offset;
	int Value = -100;
	
	for (Offset = 0; Offset < 200; Offset++) {
		Array[Offset] = Value++;
	}
	
	const char * File = "Data/Int8Sequence";
	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 200; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	int8_t RIn;
	
	for (Offset = 0; Offset < 200; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestInt8Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestInt8Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestInt8Random()
{
	int8_t Array[100];

	srand(time(0));
	
	int Offset;

	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = (rand() % 0xff);
		Array[Offset] *= (rand() % 2) == 0 ? 1 : -1;
	}
	
	const char * File = "Data/Int8Random";

	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);

	int8_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestInt8Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestInt8Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestUInt16Zero()
{
	const char * File = "Data/UInt16File0";

	ofstream OutFile(File);
	OCborStream Out(OutFile);
	uint16_t Zero = 0;

	Out << Zero;
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	uint16_t Got;
	
	In >> Got;

	if (Got == Zero) {
		printf("TestUInt16:Expected %d, got  %d\n", Zero, Got);
	} else {
		printf("ERROR:TestUInt16:Expected %d, got  %d\n", Zero, Got);
	}

	CPPUNIT_ASSERT(Zero == Got);

	InFile.close();

	return;
}

void
JustInts::TestUInt16Sequence()
{
	uint16_t Array[100];

	int Offset;
	
	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = Offset;
	}
	
	const char * File = "Data/UInt16Sequence";
	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	uint16_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestUInt16Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestUInt16Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestUInt16Random()
{
	uint16_t Array[100];

	srand(time(0));
	
	int Offset;

	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = (rand() % 0xffff);
	}
	
	const char * File = "Data/UInt16Random";

	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);

	uint16_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestUInt16Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestUInt16Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestInt16Zero()
{
	const char * File = "Data/Int16File0";

	ofstream OutFile(File);
	OCborStream Out(OutFile);
	int16_t Zero = 0;

	Out << Zero;
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	int16_t Got;
	
	In >> Got;

	if (Got == Zero) {
		printf("TestInt16:Expected %d, got  %d\n", Zero, Got);
	} else {
		printf("ERROR:TestInt16:Expected %d, got  %d\n", Zero, Got);
	}

	CPPUNIT_ASSERT(Zero == Got);

	InFile.close();

	return;
}

void
JustInts::TestInt16Sequence()
{
	int16_t Array[200];

	int Offset;
	int Value = -100;
	
	for (Offset = 0; Offset < 200; Offset++) {
		Array[Offset] = Value++;
	}
	
	const char * File = "Data/Int16Sequence";
	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 200; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	int16_t RIn;
	
	for (Offset = 0; Offset < 200; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestUInt16Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestUInt16Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestInt16Random()
{
	int16_t Array[100];

	srand(time(0));
	
	int Offset;

	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = (rand() % 0xffff);
		//		Array[Offset] *= (rand() % 2) == 0 ? 1 : -1;
	}
	
	const char * File = "Data/Int16Random";

	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}
	
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);

	int16_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestInt16Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestInt16Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestUInt32Zero()
{
	const char * File = "Data/Int16File0";

	ofstream OutFile(File);
	OCborStream Out(OutFile);
	int32_t Zero = 0;

	Out << Zero;
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	int32_t Got;
	
	In >> Got;
	printf("TestInt32Random:Expected %d, got  %d\n", Zero, Got);
	CPPUNIT_ASSERT(Zero == Got);
	InFile.close();
	
	return;
}

void
JustInts::TestUInt32Sequence()
{
	uint32_t Array[100];

	int Offset;
	
	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = Offset;
	}
	
	const char * File = "Data/UInt32Sequence";
	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	uint32_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestUInt32Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestUInt32Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestUInt32Random()
{
	uint32_t Array[100];

	srand(time(0));
	
	int Offset;

	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = (rand() % 0xffffffff);
	}
	
	const char * File = "Data/UInt32Random";

	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}
	
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);

	uint32_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestUInt32Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestUInt32Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestInt32Zero()
{
	const char * File = "Data/Int32File0";

	ofstream OutFile(File);
	OCborStream Out(OutFile);
	int32_t Zero = 0;

	Out << Zero;
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	int32_t Got;
	
	In >> Got;

	if (Got == Zero) {
		printf("TestInt32:Expected %d, got  %d\n", Zero, Got);
	} else {
		printf("ERROR:TestInt32:Expected %d, got  %d\n", Zero, Got);
	}

	CPPUNIT_ASSERT(Zero == Got);

	InFile.close();

	return;
}

void
JustInts::TestInt32Sequence()
{
	int32_t Array[200];

	int Offset;
	int Value = -100;
	
	for (Offset = 0; Offset < 200; Offset++) {
		Array[Offset] = Value++;
	}
	
	const char * File = "Data/Int32Sequence";
	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 200; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	int32_t RIn;
	
	for (Offset = 0; Offset < 200; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestInt32Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestInt32Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestInt32Random()
{
	int32_t Array[100];

	srand(time(0));
	
	int Offset;

	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = (rand() % 0xffffffff);
		Array[Offset] *= (rand() % 2) == 0 ? 1 : -1;
	}
	
	const char * File = "Data/Int32Random";

	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}
	
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);

	int32_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestInt32Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestInt32Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestUInt64Zero()
{
	const char * File = "Data/UInt64File0";

	ofstream OutFile(File);
	OCborStream Out(OutFile);
	uint64_t Zero = 0;

	Out << Zero;
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	int64_t Got;
	
	In >> Got;
	printf("TestUInt64Zero:Expected %d, got  %d\n", Zero, Got);
	CPPUNIT_ASSERT(Zero == Got);
	InFile.close();
	
	return;
}

void
JustInts::TestUInt64Sequence()
{
	uint64_t Array[100];
	uint64_t Offset;
	
	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = Offset + 0xffffffff;
	}
	
	const char * File = "Data/UInt64Sequence";
	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	uint64_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestUInt64Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestUInt64Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
		
}

void
JustInts::TestUInt64Random()
{
	uint64_t Array[100];

	srand(time(0));
	
	uint64_t Offset;
	uint64_t Tmp;
	
	for (Offset = 0; Offset < 100; Offset++) {
		Tmp  = ((uint64_t)rand() << 32);
		Tmp |= rand();
		Array[Offset] = Tmp;
	}
	
	const char * File = "Data/UInt64Random";

	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);

	uint64_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestUInt64Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestUInt64Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestInt64Zero()
{
	const char * File = "Data/Int64File0";

	ofstream OutFile(File);
	OCborStream Out(OutFile);
	int64_t Zero = 0;

	Out << Zero;
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	int64_t Got;
	
	In >> Got;

	if (Got == Zero) {
		printf("TestInt64:Expected %d, got  %d\n", Zero, Got);
	} else {
		printf("ERROR:TestInt64:Expected %d, got  %d\n", Zero, Got);
	}

	CPPUNIT_ASSERT(Zero == Got);

	InFile.close();

	return;
}

void
JustInts::TestInt64Sequence()
{
	int64_t Array[200];

	int64_t Offset;
	uint64_t Value = 0xffffffffffffffff;
	
	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = Value++;
	}
	Value = 0xffffffff + 1;
	
	for (Offset = 0; Offset < 100; Offset++) {
		Array[Offset] = Value++;
	}
	
	const char * File = "Data/UInt8Sequence";
	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 200; Offset++) {
		Out << Array[Offset];
	}

	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);
	int64_t RIn;
	
	for (Offset = 0; Offset < 200; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestInt64Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestInt64Sequence:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

void
JustInts::TestInt64Random()
{
	int64_t Array[100];

	srand(time(0));
	
	int Offset;
	int64_t Tmp;
	
	for (Offset = 0; Offset < 100; Offset++) {
		do {
			Tmp  = ((uint64_t)rand() << 32);
			Tmp |= rand();
		} while (Tmp == 0 || Tmp <= 0xffffffff);
		Array[Offset] = Tmp;
		Array[Offset] *= (rand() % 2) == 0 ? 1 : -1;
	}
	
	const char * File = "Data/Int64Random";

	ofstream OutFile(File);
	OCborStream Out(OutFile);

	for (Offset = 0; Offset < 100; Offset++) {
		Out << Array[Offset];
	}
	
	OutFile.close();

	ifstream InFile(File);
	ICborStream In(InFile);

	int64_t RIn;
	
	for (Offset = 0; Offset < 100; Offset++) {
		In >> RIn;

		if (RIn == Array[Offset]) {
			printf("TestInt64Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		} else {
			printf("ERROR:TestInt64Random:At %d: Expected %d, got  %d\n",
						 Offset,
						 Array[Offset],
						 RIn);
		}
		CPPUNIT_ASSERT(RIn == Array[Offset]);
	}

	InFile.close();

	return;
}

int
main(int argc, char ** argv)
{

	CppUnit::Test * Suite
		= CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  // Adds the test to the list of test to run.
	//
  CppUnit::TextUi::TestRunner Runner;

  Runner.addTest(Suite);

	CppUnit::TestResult Controller;
	CppUnit::TestResultCollector Result;

	Controller.addListener(&Result);

	CustomTestListener Listener;
	Controller.addListener(&Listener);

	Runner.eventManager().addListener(&Listener);
	Runner.eventManager().addListener(&Result);
	
  // Change the default outputter to a compiler error format outputter.
	//
  Runner.setOutputter(new CppUnit::CompilerOutputter(&Runner.result(),
																										 std::cerr));
  // Run the tests.
	//
  bool wasSucessful = Runner.run();

  // Return error code 1 if the one of test failed.
	//
  return wasSucessful ? 0 : 1;
}
