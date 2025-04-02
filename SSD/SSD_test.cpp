#include "gmock/gmock.h"
#include <string>
#include <map>
#include <fstream>
#include <cstdio>

#include "ssd.h"

using namespace testing;

class SSDFixture : public Test {
protected:
	void SetUp() override {
		deleteOutPutFile();
		deleteNandFile();
	}

	void TearDown() override {

	}

	void validCheckOfOutputFile(std::string expected) {
		std::ifstream file(outputFilePath.data());
		std::string actual = "";

		ASSERT_EQ(true, file.good());

		std::getline(file, actual);
		file.close();

		EXPECT_EQ(expected, actual);
	}

	void deleteFile(std::string file) {
		remove(file.c_str());
	}

	void deleteOutPutFile() {
		deleteFile(outputFilePath);
	}

	void deleteNandFile() {
		deleteFile(nandFilePath);
	}

	void prepareRead() {
		std::ofstream file(nandFilePath.c_str());
		for (auto& data : nand) {
			file << data.first << " " << data.second;
		}
		file.close();
	}

	std::map<int, std::string> nand = {
	{0, "0xAAAAAAAA"},
	{1, "0xBBBBBBBB"},
	{99, "0xFFFFFFFF"},
	};

	SSD mySsd;
	ReadSSD myRead;
	WriteSSD myWrite;

	const std::string outputFilePath = "ssd_output.txt";
	const std::string nandFilePath = "ssd_nand.txt";
};

TEST_F(SSDFixture, SSDTest_ValidWriteCommand)
{
	bool ret = mySsd.run("W 0 0x12345678");
	EXPECT_EQ(ret, true);
}

TEST_F(SSDFixture, SSDTest_ValidReadCommand)
{
	bool ret = mySsd.run("W 0 0x12345678");
	EXPECT_EQ(ret, true);

	ret = mySsd.run("R 0");
	validCheckOfOutputFile("0x12345678");
}

TEST_F(SSDFixture, SSDTest_InvalidCommand)
{
	bool ret = mySsd.run("D 0");
	EXPECT_EQ(ret, false);
}

TEST_F(SSDFixture, SSDTest_InvalidLBAWriteCommand1)
{
	bool ret = mySsd.run("W 100 0xCAFECAFE");
	EXPECT_EQ(ret, false);
	validCheckOfOutputFile("ERROR");
}

TEST_F(SSDFixture, SSDTest_InvalidLBAWriteCommand2)
{
	bool ret = mySsd.run("W !!! 0xCAFECAFE");
	EXPECT_EQ(ret, false);
	validCheckOfOutputFile("ERROR");
}

TEST_F(SSDFixture, SSDTest_InvalidLBAReadCommand1)
{
	mySsd.run("R 999");
	validCheckOfOutputFile("ERROR");
}

TEST_F(SSDFixture, SSDTest_InvalidLBAReadCommand2)
{
	mySsd.run("R !~@");
	validCheckOfOutputFile("ERROR");
}

TEST_F(SSDFixture, InvalidAddrTest)
{
	mySsd.run("R -1 0xAAAAAAAA");
	validCheckOfOutputFile("ERROR");

	mySsd.run("R -100 0xBBBBBBBB");
	validCheckOfOutputFile("ERROR");
}

TEST_F(SSDFixture, ReadFromUninitializedMemory)
{
	prepareRead();

	myRead.execute(nand, 3);
	validCheckOfOutputFile("0x00000000");

	myRead.execute(nand, 98);
	validCheckOfOutputFile("0x00000000");
}

TEST_F(SSDFixture, ReadWithDataTest)
{
	prepareRead();

	myRead.execute(nand, 0);
	validCheckOfOutputFile("0xAAAAAAAA");
}

TEST_F(SSDFixture, ReadWithDataTest2)
{
	prepareRead();

	myRead.execute(nand, 0);
	validCheckOfOutputFile("0xAAAAAAAA");

	myRead.execute(nand, 1);
	validCheckOfOutputFile("0xBBBBBBBB");

	myRead.execute(nand, 99);
	validCheckOfOutputFile("0xFFFFFFFF");
}

TEST_F(SSDFixture, WriteWithDataTest)
{
	std::map<int, std::string> nand = { };
	bool ret = myWrite.execute(nand, 2, "0xCCCCCCCC");
	EXPECT_EQ(ret, true);
}

TEST_F(SSDFixture, WriteFileUpdateTest)
{
	std::map<int, std::string> nand = { };
	myWrite.execute(nand, 2, "0xCCCCCCCC");

	std::ifstream file(nandFilePath.c_str());
	std::string output = "";
	std::getline(file, output);
	EXPECT_EQ("2 0xCCCCCCCC", output);
}

TEST_F(SSDFixture, WriteFileUpdateChangeMapValueTest)
{
	WriteSSD myWrite;
	std::map<int, std::string> nand = { };

	myWrite.execute(nand, 2, "0xCCCCCCCC");
	myWrite.execute(nand, 2, "0xDDDDDDDD");

	std::ifstream file(nandFilePath.c_str());
	std::string output = "";
	std::getline(file, output);
	EXPECT_EQ("2 0xDDDDDDDD", output);
}