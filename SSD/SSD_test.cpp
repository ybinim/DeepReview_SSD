#include "gmock/gmock.h"
#include <string>
#include <map>
#include <fstream>
#include "SSD_Read.cpp"

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

TEST(SSDTestGroup, ReadWithDataTestInvalidAddr)
{
	Read myRead;
	std::map<int, std::string> nand = {
		{-1, "0xAAAAAAAA"},
	};
	std::string filePath = "ssd_output.txt";
	std::ifstream file(filePath.data());
	std::string output = "";

	myRead.execute(nand, -1);

	ASSERT_EQ(true, file.good());

	std::getline(file, output);
	file.close();

	EXPECT_EQ("ERROR", output);
}

TEST(SSDTestGroup, ReadWithDataTestNotExistKey)
{
	Read myRead;
	std::map<int, std::string> nand = {
		{0, "0xAAAAAAAA"},
		{1, "0xBBBBBBBB"}
	};
	std::string filePath = "ssd_output.txt";
	std::ifstream file(filePath.data());
	std::string output = "";

	myRead.execute(nand, 3);

	ASSERT_EQ(true, file.good());

	std::getline(file, output);
	file.close();

	EXPECT_EQ("0x00000000", output);
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

	myRead.execute(nand, 0);
	
	ASSERT_EQ(true, file.good());

	std::getline(file, output);
	file.close();

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

	myRead.execute(nand, 0);

	ASSERT_EQ(true, file.good());

	std::getline(file, output);
	file.close();
	EXPECT_EQ("0xAAAAAAAA", output);

	std::ifstream file2(filePath.data());
	myRead.execute(nand, 1);

	std::getline(file2, output);
	file2.close();
	EXPECT_EQ("0xBBBBBBBB", output);
}
