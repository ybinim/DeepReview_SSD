#include "gmock/gmock.h"
#include "Testshell.h"
#include "SSDRunner.h"

#include <string>
#include <vector>
#include <fstream>

using namespace std;
using namespace testing;

map<int, string> nandText;
string outputText;


class MockReader : public SSDRunner
{
public:
	int execute(vector<string>& param) override {
		if (param.size() != 2) {
			return -2;
		}

		string lba = param[1];
		if (lba.length() > 2 || isNumber(lba) == false) {
			return -2;
		}

		outputText = "0x00000000";
		auto it = nandText.find(stoi(lba));
		if (it != nandText.end()) {
			outputText = it->second;
		}

		return 0;
	}
};

class MockWriter : public SSDRunner
{
public:
	int execute(vector<string>& param) override {
		if (param.size() != 3) {
			return -2;
		}

		string lba = param[1];
		string data = param[2];

		if (lba.length() > 2 || isNumber(lba) == false) {
			return -2;
		}

		if (data.length() != 10) {
			return -2;
		}

		if (data.substr(0, 2).compare("0x") != 0) {
			return -2;
		}

		for (char& c : data.substr(2, string::npos)) {
			if (c < 'A' || c > 'F') {
				return -2;
			}
		}

		auto it = nandText.find(stoi(lba));
		if (it != nandText.end()) {
			it->second = data;
		}
		else {
			nandText.emplace(stoi(lba), data);
		}
		return 0;
	}
};

class TestShellTestFixture : public Test
{
public:
	MockReader reader;
	MockWriter writer;
	TestShell* shell;

protected:
	void SetUp() override {
		shell = new TestShell(&reader, &writer);
		nandText.clear();
		outputText = "";
	}

	void TearDown() override {
		delete shell;
	}
};


TEST_F(TestShellTestFixture, ExitTest)
{
	int ret = shell->run("exit");
	EXPECT_EQ(ret, 1);
}

TEST_F(TestShellTestFixture, InvalidCommandTest)
{
	int ret;

	ret = shell->run("move");
	EXPECT_EQ(ret, -1);

	ret = shell->run("copy");
	EXPECT_EQ(ret, -1);
}

TEST_F(TestShellTestFixture, WriteTest)
{
	int ret = shell->run("write 0 0xAAAAAAAA");
	EXPECT_EQ(ret, 0);

	auto it = nandText.find(0);
	EXPECT_TRUE(it != nandText.end());
	EXPECT_EQ(it->second, "0xAAAAAAAA");

	ret = shell->run("write 1 0xBBBBBBBB");
	EXPECT_EQ(ret, 0);

	it = nandText.find(1);
	EXPECT_TRUE(it != nandText.end());
	EXPECT_EQ(it->second, "0xBBBBBBBB");

	ret = shell->run("write 1 0xCCCCCCCC");
	EXPECT_EQ(ret, 0);

	it = nandText.find(1);
	EXPECT_TRUE(it != nandText.end());
	EXPECT_EQ(it->second, "0xCCCCCCCC");
}

TEST_F(TestShellTestFixture, WrongWriteTest)
{
	int ret = shell->run("write 3");
	EXPECT_EQ(ret, -2);

	ret = shell->run("write 3 0xABCDEFGHIJKLMN");
	EXPECT_EQ(ret, -2);

	ret = shell->run("write 0a 0xAAAAAAAA");
	EXPECT_EQ(ret, -2);

	ret = shell->run("write 100 0xAAAAAAAA");
	EXPECT_EQ(ret, -2);
}

TEST_F(TestShellTestFixture, ReadTest)
{
	int ret = shell->run("read 0");
	EXPECT_EQ(ret, 0);
	EXPECT_EQ(outputText, "0x00000000");
}

TEST_F(TestShellTestFixture, ReadTest2)
{
	nandText.emplace(0, "0xAAAAAAAA");
	nandText.emplace(1, "0xBBBBBBBB");
	nandText.emplace(99, "0xFFFFFFFF");

	int ret = shell->run("read 0");
	EXPECT_EQ(ret, 0);
	EXPECT_EQ(outputText, "0xAAAAAAAA");

	ret = shell->run("read 1");
	EXPECT_EQ(ret, 0);
	EXPECT_EQ(outputText, "0xBBBBBBBB");

	ret = shell->run("read 99");
	EXPECT_EQ(ret, 0);
	EXPECT_EQ(outputText, "0xFFFFFFFF");
}

TEST_F(TestShellTestFixture, WrongReadTest)
{
	int ret = shell->run("read 0a");
	EXPECT_EQ(ret, -2);

	ret = shell->run("read 100");
	EXPECT_EQ(ret, -2);

	ret = shell->run("read 0 0xAAAAAAAA");
	EXPECT_EQ(ret, -2);
}

TEST_F(TestShellTestFixture, HelpCommandTest) {
	int ret = shell->run("help");
	EXPECT_EQ(ret, 0);
}

TEST_F(TestShellTestFixture, FullWriteTest)
{
	int ret = shell->run("fullwrite 0xFFFFFFFF");
	EXPECT_EQ(ret, 0);

	EXPECT_EQ(nandText.size(), 100);
	for (int i = 0; i < 100; i++) {
		auto it = nandText.find(i);
		ASSERT_TRUE(it != nandText.end());
		EXPECT_EQ(it->second, "0xFFFFFFFF");
	}
}

TEST_F(TestShellTestFixture, InvalidFullWriteTest)
{
	int ret = shell->run("fullwrite");
	EXPECT_EQ(ret, -2);

	ret = shell->run("fullwrite AAAAAAAAAA");
	EXPECT_EQ(ret, -2);

	ret = shell->run("fullwrite 12345678");
	EXPECT_EQ(ret, -2);
}

TEST_F(TestShellTestFixture, WriteReadAgingValidTest)
{
	int ret = shell->run("3_WriteReadAging");
	EXPECT_EQ(ret, 0);

	ret = shell->run("3_");
	EXPECT_EQ(ret, 0);

	ret = shell->run("3");
	EXPECT_EQ(ret, -1);

	ret = shell->run("3__");
	EXPECT_EQ(ret, -1);
}
