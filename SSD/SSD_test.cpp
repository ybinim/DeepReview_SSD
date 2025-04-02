#include "gmock/gmock.h"
#include <string>
#include <map>
#include <fstream>
#include "ssd.h"
#include "SSD_Read.cpp"
#include "write.h"

using namespace testing;

TEST(SSDTestGroup, ValidReadCommandTest)
{
	SSD mySsd;
	bool ret = mySsd.run("R 0");
	EXPECT_EQ(ret, true);
}

TEST(SSDTestGroup, ValidWriteCommandTest)
{
	SSD mySsd;
	bool ret = mySsd.run("W 0 0x12345678");
	EXPECT_EQ(ret, true);
}

TEST(SSDTestGroup, InvalidCommandTest)
{
	SSD mySsd;
	bool ret = mySsd.run("D 0");
	EXPECT_EQ(ret, false);
}

class ReadSSDFixture : public Test {
public:
	void ValidCheckOfOutputFile(std::string expected) {
		std::ifstream file(outputFilePath.data());
		std::string actual = "";

		ASSERT_EQ(true, file.good());

		std::getline(file, actual);
		file.close();

		EXPECT_EQ(expected, actual);
	}

	std::map<int, std::string> nand = {
	{0, "0xAAAAAAAA"},
	{1, "0xBBBBBBBB"},
	{99, "0xFFFFFFFF"},
	};

	ReadSSD myRead;
private:
	const std::string outputFilePath = "ssd_output.txt";
};

TEST_F(ReadSSDFixture, ReadInvalidAddrTest)
{
	std::map<int, std::string> invalidAddr = {
		{-1, "0xAAAAAAAA"},
		{100, "0xAAAAAAAA"},
	};

	myRead.execute(invalidAddr, -1);

	ValidCheckOfOutputFile("ERROR");

	myRead.execute(invalidAddr, 100);

	ValidCheckOfOutputFile("ERROR");
}

TEST_F(ReadSSDFixture, ReadFromUninitializedMemory)
{
	myRead.execute(nand, 3);

	ValidCheckOfOutputFile("0x00000000");

	myRead.execute(nand, 98);

	ValidCheckOfOutputFile("0x00000000");
}

TEST_F(ReadSSDFixture, ReadWithDataTest)
{
	myRead.execute(nand, 0);

	ValidCheckOfOutputFile("0xAAAAAAAA");
}

TEST_F(ReadSSDFixture, ReadWithDataTest2)
{
	myRead.execute(nand, 0);

	ValidCheckOfOutputFile("0xAAAAAAAA");

	myRead.execute(nand, 1);

	ValidCheckOfOutputFile("0xBBBBBBBB");

	myRead.execute(nand, 99);

	ValidCheckOfOutputFile("0xFFFFFFFF");
}

TEST(SSDTestGroup, WriteWithDataTest)
{
	WriteSSD myWrite;
	std::map<int, std::string> nand = { };
	bool ret = myWrite.execute(nand, 2, "0xCCCCCCCC");
	EXPECT_EQ(ret, true);
}

TEST(SSDTestGroup, WriteFileUpdateTest)
{
	WriteSSD myWrite;
	std::map<int, std::string> nand = { };
	std::string filePath = "ssd_nand.txt";
	std::ifstream file(filePath.data());
	std::string output = "";

	myWrite.execute(nand, 2, "0xCCCCCCCC");
	std::getline(file, output);
	EXPECT_EQ("2 0xCCCCCCCC", output);
}

TEST(SSDTestGroup, WriteFileUpdateChangeMapValueTest)
{
	WriteSSD myWrite;
	std::map<int, std::string> nand = { };
	std::string filePath = "ssd_nand.txt";
	std::ifstream file(filePath.data());
	std::string output = "";

	myWrite.execute(nand, 2, "0xCCCCCCCC");
	myWrite.execute(nand, 2, "0xDDDDDDDD");
	std::getline(file, output);
	EXPECT_EQ("2 0xDDDDDDDD", output);
}

