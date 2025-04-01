#include "gmock/gmock.h"
#include <string>
#include <map>
#include <fstream>

#include "ssd.cpp"

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