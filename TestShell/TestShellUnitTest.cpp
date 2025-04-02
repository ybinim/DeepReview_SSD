#include "gmock/gmock.h"
#include "TestShell.cpp"

#include <string>
#include <fstream>

using namespace std;
using namespace testing;

class TestShellTestFixture : public Test
{
public:
	TestShell shell;
	const string nandTxt = "../SSD/ssd_nand.txt";
	const string outputTxt = "../SSD/ssd_output.txt";
	
protected:
	void SetUp() override {
		remove(nandTxt.c_str());
		remove(outputTxt.c_str());
	}

	void TearDown() override {
		remove(nandTxt.c_str());
		remove(outputTxt.c_str());
	}
};

TEST(TestShellTestGroup, TestShellParamTest)
{
	TestShell shell;
	int ret;

	ret = shell.run("read 3");
	EXPECT_EQ(ret, 0);

	ret = shell.run("help");
	EXPECT_EQ(ret, 0);

	ret = shell.run("fullwrite 0xBBBBBBBB");
	EXPECT_EQ(ret, 0);

	ret = shell.run("fullread");
	EXPECT_EQ(ret, 0);

	ret = shell.run("exit");
	EXPECT_EQ(ret, 1);
}

TEST(TestShellTestGroup, TestShellInvalidCommand)
{
	TestShell shell;
	int ret;

	ret = shell.run("move");
	EXPECT_EQ(ret, -1);

	ret = shell.run("copy");
	EXPECT_EQ(ret, -1);
}

TEST_F(TestShellTestFixture, WriteTest)
{
	int ret = shell.run("write 3 0xAAAAAAAA");
	ASSERT_EQ(ret, 0);

	ifstream file(nandTxt.c_str());
	ASSERT_EQ(file.is_open(), true);
	
	string data = "";
	getline(file, data);
	EXPECT_EQ(data.compare("3 0xAAAAAAAA"), 0);
}

TEST_F(TestShellTestFixture, WrongWriteTest)
{
	int ret = shell.run("write 3");
	EXPECT_EQ(ret, -2);

	ret = shell.run("write 3 0xABCDEFGHIJKLMN");
	EXPECT_EQ(ret, -2);

	ret = shell.run("write 0a 0xAAAAAAAA");
	EXPECT_EQ(ret, -2);

	ret = shell.run("write 100 0xAAAAAAAA");
	EXPECT_EQ(ret, -2);
}