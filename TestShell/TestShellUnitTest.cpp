#include "gmock/gmock.h"
#include "TestShell.h"

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

	void prepareRead() {
		shell.run("write 0 0xAAAAAAAA");
		shell.run("write 1 0xBBBBBBBB");
		shell.run("write 99 0xFFFFFFFF");
	}

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

TEST_F(TestShellTestFixture, ExitTest)
{
	TestShell shell;
	int ret = shell.run("exit");
	EXPECT_EQ(ret, 1);
}

TEST_F(TestShellTestFixture, InvalidCommandTest)
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

	ifstream file;
	file.open(nandTxt.c_str());
	ASSERT_EQ(file.good(), true);

	string data = "";
	getline(file, data);
	EXPECT_EQ(data.compare("3 0xAAAAAAAA"), 0);
	file.close();
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

	ifstream file(nandTxt.c_str());
	EXPECT_EQ(file.good(), false);
}

TEST_F(TestShellTestFixture, ReadTest)
{
	int ret = shell.run("read 0");
	EXPECT_EQ(ret, 0);

	ifstream file;
	file.open(outputTxt.c_str());
	ASSERT_EQ(file.good(), true);

	string data = "";
	getline(file, data);
	EXPECT_EQ(data.compare("0xAAAAAAAA"), 0);
	file.close();

	ret = shell.run("read 1");
	EXPECT_EQ(ret, 0);

	file.open(outputTxt.c_str());
	ASSERT_EQ(file.good(), true);

	getline(file, data);
	EXPECT_EQ(data.compare("0xBBBBBBBB"), 0);
	file.close();
}

TEST_F(TestShellTestFixture, WrongReadTest)
{
	int ret = shell.run("Read 0a");
	EXPECT_EQ(ret, -2);

	ret = shell.run("Read 100");
	EXPECT_EQ(ret, -2);

	ret = shell.run("read 0 0xAAAAAAAA");
	EXPECT_EQ(ret, -2);

	ifstream file(outputTxt.c_str());
	EXPECT_EQ(file.good(), false);
}