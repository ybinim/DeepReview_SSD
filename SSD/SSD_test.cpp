#include "gmock/gmock.h"
#include <string>
#include <map>
#include <fstream>
#include <cstdio>
#include <filesystem>

#include "ssd.h"

using namespace testing;

class SSDFixture : public Test {
protected:
	void SetUp() override {
		initCommandBuffer();
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

	void initCommandBuffer() {
		if (std::filesystem::exists(bufferDirPath)) {
			std::filesystem::remove_all(bufferDirPath);
		}
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
	EraseSSD myErase;

	const std::string outputFilePath = "ssd_output.txt";
	const std::string nandFilePath = "ssd_nand.txt";
	const std::string bufferDirPath = "buffer";
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
	bool ret = myWrite.execute(nand, 2, "0xCCCCCCCC");
	EXPECT_EQ(ret, true);
}

TEST_F(SSDFixture, WriteFileUpdateTest)
{
	std::map<int, std::string> emptyNand;
	myWrite.execute(emptyNand, 2, "0xCCCCCCCC");

	std::ifstream file(nandFilePath.c_str());
	std::string output = "";
	std::getline(file, output);
	EXPECT_EQ("2 0xCCCCCCCC", output);
}

TEST_F(SSDFixture, WriteFileUpdateChangeMapValueTest)
{
	std::map<int, std::string> emptyNand;
	myWrite.execute(emptyNand, 2, "0xCCCCCCCC");
	myWrite.execute(emptyNand, 2, "0xDDDDDDDD");

	std::ifstream file(nandFilePath.c_str());
	std::string output = "";
	std::getline(file, output);
	EXPECT_EQ("2 0xDDDDDDDD", output);
}

TEST_F(SSDFixture, ValidEraseParam)
{
	bool ret = mySsd.run("E 0 10");
	EXPECT_EQ(ret, true);

	// Size 0은 에러는 아니다. 아무런 동작을 하지 않는다.
	ret = mySsd.run("E 0 0");
	EXPECT_EQ(ret, true);

	// 99까지만 처리함.
	ret = mySsd.run("E 95 10");
	EXPECT_EQ(ret, true);
}

TEST_F(SSDFixture, InvalidEraseParam)
{
	bool ret = mySsd.run("E 0 -1");
	EXPECT_EQ(ret, false);
	validCheckOfOutputFile("ERROR");
}

TEST_F(SSDFixture, InvalidEraseParam2)
{
	bool ret = mySsd.run("E 0 11");
	EXPECT_EQ(ret, false);
	validCheckOfOutputFile("ERROR");
}

TEST_F(SSDFixture, InvalidEraseParam3)
{
	bool ret = mySsd.run("E 100 10");
	EXPECT_EQ(ret, false);
	validCheckOfOutputFile("ERROR");
}

TEST_F(SSDFixture, EraseTest_WriteAndErase)
{
	std::map<int, std::string> emptyNand;
	myWrite.execute(emptyNand, 0, "0x12345678");
	myWrite.execute(emptyNand, 1, "0xCCCCCCCC");
	myWrite.execute(emptyNand, 2, "0x1234ABCD");
	myWrite.execute(emptyNand, 3, "0xCCCCCCCC");
	myWrite.execute(emptyNand, 4, "0x00000000");
	myWrite.execute(emptyNand, 5, "0x5555AAAA");
	myWrite.execute(emptyNand, 6, "0xCCCCCCCC");

	bool ret = myErase.execute(emptyNand, 4, "10");
	EXPECT_EQ(ret, true);

	for (int i = 4; i < 14; i++)
	{
		std::string command = "R ";
		command.append(std::to_string(i));
		mySsd.run(command);
		validCheckOfOutputFile("0x00000000");
	}
}

TEST_F(SSDFixture, EraseTest_WriteAndEraseWithSizeZero)
{
	std::map<int, std::string> emptyNand;
	myWrite.execute(emptyNand, 97, "0x12345678");
	myWrite.execute(emptyNand, 98, "0xCCCCCCCC");
	myWrite.execute(emptyNand, 99, "0x1234ABCD");

	bool ret = myErase.execute(emptyNand, 1, "0");
	EXPECT_EQ(ret, true);
}

TEST_F(SSDFixture, EraseTest_WriteAndEraseWithSize1)
{
	std::map<int, std::string> emptyNand;
	myWrite.execute(emptyNand, 97, "0x12345678");
	myWrite.execute(emptyNand, 98, "0xCCCCCCCC");
	myWrite.execute(emptyNand, 99, "0x1234ABCD");

	bool ret = myErase.execute(emptyNand, 1, "1");
	EXPECT_EQ(ret, true);
}

TEST_F(SSDFixture, CommandBufferTest_Basic)
{
	bool ret = mySsd.run("W 0 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 1 0xBBBBBBBB");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("E 3 4");
	EXPECT_EQ(ret, true);

	EXPECT_EQ(std::filesystem::exists(nandFilePath), false);

	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 0 0xAAAAAAAA"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 1 0xBBBBBBBB"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/E 3 4"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/4_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/5_empty"), true);
}

TEST_F(SSDFixture, CommandBufferTest_FullBuffer)
{
	bool ret = mySsd.run("W 0 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 1 0xBBBBBBBB");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("E 3 10");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 5 0xCCCCCCCC");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 6 0xDDDDDDDD");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 10 0x10101010");
	EXPECT_EQ(ret, true);

	EXPECT_EQ(std::filesystem::exists(nandFilePath), true);
	std::string data;
	std::ifstream nandFile(nandFilePath.c_str());
	getline(nandFile, data);
	EXPECT_EQ(data, "0 0xAAAAAAAA");
	getline(nandFile, data);
	EXPECT_EQ(data, "1 0xBBBBBBBB");
	getline(nandFile, data);
	EXPECT_EQ(data, "5 0xCCCCCCCC");
	getline(nandFile, data);
	EXPECT_EQ(data, "6 0xDDDDDDDD");

	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 10 0x10101010"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/2_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/3_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/4_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/5_empty"), true);
}

TEST_F(SSDFixture, CommandBufferTest_Overwrite)
{
	bool ret = mySsd.run("W 0 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 1 0xBBBBBBBB");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 0 0x12345678");
	EXPECT_EQ(ret, true);

	EXPECT_EQ(std::filesystem::exists(nandFilePath), false);

	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 0 0x12345678"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 1 0xBBBBBBBB"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/3_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/4_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/5_empty"), true);
}

TEST_F(SSDFixture, CommandBufferTest_EraseAfterWrite1)
{
	bool ret = mySsd.run("W 0 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 1 0xBBBBBBBB");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("E 1 3");
	EXPECT_EQ(ret, true);

	EXPECT_EQ(std::filesystem::exists(nandFilePath), false);

	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 0 0xAAAAAAAA"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/E 1 3"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/3_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/4_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/5_empty"), true);
}

TEST_F(SSDFixture, CommandBufferTest_EraseAfterWrite2)
{
	bool ret = mySsd.run("W 4 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 5 0xBBBBBBBB");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("E 0 10");
	EXPECT_EQ(ret, true);

	EXPECT_EQ(std::filesystem::exists(nandFilePath), false);

	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/E 0 10"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/2_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/3_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/4_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/5_empty"), true);
}

TEST_F(SSDFixture, CommandBufferTest_EraseWideRange)
{
	bool ret = mySsd.run("W 0 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 1 0xBBBBBBBB");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("E 3 4");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("E 3 7");
	EXPECT_EQ(ret, true);

	EXPECT_EQ(std::filesystem::exists(nandFilePath), false);

	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 0 0xAAAAAAAA"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 1 0xBBBBBBBB"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/E 3 7"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/4_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/5_empty"), true);
}

TEST_F(SSDFixture, CommandBufferTest_EraseNarrowRange)
{
	bool ret = mySsd.run("W 0 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 1 0xBBBBBBBB");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("E 3 4");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("E 4 1");
	EXPECT_EQ(ret, true);

	EXPECT_EQ(std::filesystem::exists(nandFilePath), false);

	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 0 0xAAAAAAAA"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 1 0xBBBBBBBB"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/E 3 4"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/4_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/5_empty"), true);
}

TEST_F(SSDFixture, CommandBufferTest_ReadFromCommandBuffer)
{
	bool ret = mySsd.run("W 0 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 1 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 2 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 3 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 4 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 0 0xCCCCCCCC");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("R 0");
	EXPECT_EQ(ret, true);

	EXPECT_EQ(std::filesystem::exists(nandFilePath), true);
	EXPECT_EQ(std::filesystem::exists(outputFilePath), true);
	std::string data;
	std::ifstream outputFile;
	outputFile.open(outputFilePath.c_str());
	getline(outputFile, data);
	outputFile.close();
	EXPECT_EQ(data, "0xCCCCCCCC");

	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 0 0xCCCCCCCC"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/2_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/3_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/4_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/5_empty"), true);

	ret = mySsd.run("R 1");
	EXPECT_EQ(ret, true);
	outputFile.open(outputFilePath.c_str());
	getline(outputFile, data);
	outputFile.close();
	EXPECT_EQ(data, "0xAAAAAAAA");
}

TEST_F(SSDFixture, CommandBufferTest_ReadFromCommandBuffer2)
{
	bool ret = mySsd.run("W 0 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 1 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 2 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 3 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 4 0xAAAAAAAA");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("W 0 0xCCCCCCCC");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("E 1 10");
	EXPECT_EQ(ret, true);
	ret = mySsd.run("R 0");
	EXPECT_EQ(ret, true);

	EXPECT_EQ(std::filesystem::exists(nandFilePath), true);
	EXPECT_EQ(std::filesystem::exists(outputFilePath), true);
	std::string data;
	std::ifstream outputFile;
	outputFile.open(outputFilePath.c_str());
	getline(outputFile, data);
	outputFile.close();
	EXPECT_EQ(data, "0xCCCCCCCC");

	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/W 0 0xCCCCCCCC"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/E 1 10"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/3_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/4_empty"), true);
	EXPECT_EQ(std::filesystem::exists(bufferDirPath + "/5_empty"), true);

	ret = mySsd.run("R 1");
	EXPECT_EQ(ret, true);
	outputFile.open(outputFilePath.c_str());
	getline(outputFile, data);
	outputFile.close();
	EXPECT_EQ(data, "0x00000000");
}