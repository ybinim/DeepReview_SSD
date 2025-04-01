#include "gmock/gmock.h"
#include <string>
#include <map>
#include <fstream>

#include "ssd.h"
#include "write.cpp"

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

TEST(SSDTestGroup, ReadWithDataTest)
{
	Read myRead;
	std::map<int, std::string> nand = {
		{0, "0xAAAAAAAA"},
		{1, "0xBBBBBBBB"}
	};
	std::string filePath = "ssd_output.txt";
	std::ifstream file(filePath.data());
	std::string output = "";

	myRead.execute(&nand, "0");
	
	ASSERT_EQ(true, file.good());

	std::getline(file, output);

	EXPECT_EQ("0xAAAAAAAA", output);
}

TEST(SSDTestGroup, ReadWithDataTest2)
{
	Read myRead;
	std::map<int, std::string> nand = {
		{0, "0xAAAAAAAA"},
		{1, "0xBBBBBBBB"}
	};
	std::string filePath = "ssd_output.txt";
	std::ifstream file(filePath.data());
	std::string output = "";

	myRead.execute(&nand, "0");

	ASSERT_EQ(true, file.good());

	std::getline(file, output);

	EXPECT_EQ("0xAAAAAAAA", output);

	myRead.execute(&nand, "1");

	std::getline(file, output);

	EXPECT_EQ("0xBBBBBBBB", output);
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

	bool ret = myWrite.update(nand);
	EXPECT_EQ(ret, true);

	std::getline(file, output);
	EXPECT_EQ("2 0xCCCCCCCC", output);
}
