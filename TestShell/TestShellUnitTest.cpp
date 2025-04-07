#include "gmock/gmock.h"
#include "Testshell.h"
#include "SSDExecutor.h"

#include <string>
#include <vector>
#include <fstream>

using namespace std;
using namespace testing;

map<int, string> nandText;
string outputText;


class MockReader : public SSDExecutor
{
public:
	int execute(vector<string>& param, bool print2Console = true) override {
		if (checkParam(param) != 0) {
			return -2;
		}

		outputText = "0x00000000";
		auto it = nandText.find(stoi(param[1]));
		if (it != nandText.end()) {
			outputText = it->second;
		}

		return 0;
	}

	int checkParam(vector<string>& param) override {
		if (param.size() != 2) {
			return -2;
		}

		string lba = param[1];
		if (lba.length() > 2 || isNumber(lba) == false) {
			return -2;
		}
		return 0;
	}
};

class MockWriter : public SSDExecutor
{
public:
	int execute(vector<string>& param, bool print2Console = true) override {
		if (checkParam(param) != 0) {
			return -2;
		}

		string lba = param[1];
		string data = param[2];

		auto it = nandText.find(stoi(lba));
		if (it != nandText.end()) {
			it->second = data;
		}
		else {
			nandText.emplace(stoi(lba), data);
		}
		return 0;
	}

	int checkParam(vector<string>& param) override {
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
			if (!(c >= '0' && c <= '9') && !(c >= 'A' && c <= 'F')) {
				return -2;
			}
		}
		return 0;
	}
};

class MockEraser : public SSDExecutor
{
public:
	int execute(vector<string>& param, bool print2Console) override {
		if (checkParam(param) != 0) {
			return -2;
		}

		int lba = stoi(param[1]);
		int size;
		if (param[0].compare("erase") == 0) {
			size = stoi(param[2]);
		}
		else {
			size = stoi(param[2]) - stoi(param[1]) + 1;
		}

		int ret = erase(lba, size);
		return ret;
	}

	int checkParam(vector<string>& param) override {
		if (param.size() != 3) {
			return -2;
		}

		int lba = 0;
		int size = 0;

		if (param[0].compare("erase") == 0) {
			if (param[1].length() > 2 || isNumber(param[1]) == false) {
				return -2;
			}
			if (isNumber(param[2]) == false) {
				return -2;
			}

			lba = stoi(param[1]);
			size = stoi(param[2]);
		}
		else if (param[0].compare("erase_range") == 0) {
			if (param[1].length() > 2 || isNumber(param[1]) == false) {
				return -2;
			}
			if (param[2].length() > 2 || isNumber(param[2]) == false) {
				return -2;
			}

			lba = stoi(param[1]);
			size = stoi(param[2]) - lba + 1;
			if (size < 0) {
				return -2;
			}
		}

		return 0;
	}

	int erase(int lba, int size) {
		while ((size > 0) && (lba >= 0) && (lba < 100)) {
			nandText.erase(lba);
			lba++;
			size--;
		}
		return 0;
	}
};

class MockFlusher : public SSDExecutor
{
public:
	int execute(vector<string>& param, bool print2Console = true) override {
		if (checkParam(param) != 0) {
			return -2;
		}
		return 0;
	}

	int checkParam(vector<string>& param) override {
		if (param.size() != 1) {
			return -2;
		}

		return 0;
	}
};

class TestShellTestFixture : public Test
{
public:
	MockReader reader;
	MockWriter writer;
	MockEraser eraser;
	MockFlusher flusher;
	TestScriptCallback cb;
	TestShell* shell;

protected:
	void SetUp() override {

		// 2. 콜백 구조체 세팅
		cb.reader = &reader;
		cb.writer = &writer;
		cb.eraser = &eraser;
		cb.flusher = &flusher;

		shell = new TestShell(&reader, &writer, &eraser, &flusher, &cb);
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



TEST_F(TestShellTestFixture, FullReadTestNotrecorded)
{
	int ret = shell->run("fullread");
	EXPECT_EQ(ret, 0);

	EXPECT_EQ(nandText.size(), 0);
	EXPECT_EQ(nandText.empty(), true);
	EXPECT_EQ(outputText, "0x00000000");
}

TEST_F(TestShellTestFixture, FullReadTestAfterFullWrite)
{
	shell->run("fullwrite 0xAAAAAAAA");
	int ret = shell->run("fullread");
	EXPECT_EQ(ret, 0);

	EXPECT_EQ(nandText.size(), 100);

	for (int i = 0; i < 100; i++) {
		auto it = nandText.find(i);
		ASSERT_TRUE(it != nandText.end());
		EXPECT_EQ(it->second, "0xAAAAAAAA");
	}
}

TEST_F(TestShellTestFixture, EraseParamTest)
{
	int ret = shell->run("erase 0 10");
	EXPECT_EQ(ret, 0);

	ret = shell->run("erase 99 10");
	EXPECT_EQ(ret, 0);

	ret = shell->run("erase 0 100");
	EXPECT_EQ(ret, 0);

	ret = shell->run("erase 0 0");
	EXPECT_EQ(ret, 0);

	ret = shell->run("erase -1 5");
	EXPECT_EQ(ret, -2);

	ret = shell->run("erase 0 a");
	EXPECT_EQ(ret, -2);
}

TEST_F(TestShellTestFixture, EraseRangeParamTest)
{
	int ret = shell->run("erase_range 0 10");
	EXPECT_EQ(ret, 0);

	ret = shell->run("erase_range 10 10");
	EXPECT_EQ(ret, 0);

	ret = shell->run("erase_range 5 99");
	EXPECT_EQ(ret, 0);

	ret = shell->run("erase_range -1 5");
	EXPECT_EQ(ret, -2);

	ret = shell->run("erase_range 99 100");
	EXPECT_EQ(ret, -2);

	ret = shell->run("erase_range 10 8");
	EXPECT_EQ(ret, -2);
}

TEST_F(TestShellTestFixture, EraseTest)
{
	// precondition
	for (int i = 0; i < 100; i++) {
		nandText.emplace(i, "0xAAAAAAAA");
	}

	// erase nothing
	int ret = shell->run("erase 0 0");
	EXPECT_EQ(ret, 0);

	for (int i = 0; i <= 99; i++) {
		EXPECT_EQ(nandText.find(i)->second, "0xAAAAAAAA");
	}

	// erase LBA 5~9
	ret = shell->run("erase 5 5");
	EXPECT_EQ(ret, 0);

	for (int i = 0; i <= 4; i++) {
		EXPECT_EQ(nandText.find(i)->second, "0xAAAAAAAA");
	}
	for (int i = 5; i <= 9; i++) {
		EXPECT_EQ(nandText.find(i), nandText.end());
	}
	for (int i = 10; i <= 99; i++) {
		EXPECT_EQ(nandText.find(i)->second, "0xAAAAAAAA");
	}

	// erase LBA 8~99
	ret = shell->run("erase 8 100");
	EXPECT_EQ(ret, 0);

	for (int i = 0; i <= 4; i++) {
		EXPECT_EQ(nandText.find(i)->second, "0xAAAAAAAA");
	}
	for (int i = 5; i <= 99; i++) {
		EXPECT_EQ(nandText.find(i), nandText.end());
	}
}

TEST_F(TestShellTestFixture, EraseRangeTest)
{
	// precondition
	for (int i = 0; i < 100; i++) {
		nandText.emplace(i, "0xAAAAAAAA");
	}

	// erase LBA 0
	int ret = shell->run("erase_range 0 0");
	EXPECT_EQ(ret, 0);

	EXPECT_EQ(nandText.find(0), nandText.end());
	for (int i = 1; i <= 99; i++) {
		EXPECT_EQ(nandText.find(i)->second, "0xAAAAAAAA");
	}

	// erase LBA 5
	ret = shell->run("erase_range 5 5");
	EXPECT_EQ(ret, 0);

	EXPECT_EQ(nandText.find(0), nandText.end());
	for (int i = 1; i <= 4; i++) {
		EXPECT_EQ(nandText.find(i)->second, "0xAAAAAAAA");
	}
	EXPECT_EQ(nandText.find(5), nandText.end());
	for (int i = 6; i <= 99; i++) {
		EXPECT_EQ(nandText.find(i)->second, "0xAAAAAAAA");
	}

	// erase LBA 10~99
	ret = shell->run("erase_range 10 99");
	EXPECT_EQ(ret, 0);

	EXPECT_EQ(nandText.find(0), nandText.end());
	for (int i = 1; i <= 4; i++) {
		EXPECT_EQ(nandText.find(i)->second, "0xAAAAAAAA");
	}
	EXPECT_EQ(nandText.find(5), nandText.end());
	for (int i = 6; i <= 9; i++) {
		EXPECT_EQ(nandText.find(i)->second, "0xAAAAAAAA");
	}
	for (int i = 10; i <= 99; i++) {
		EXPECT_EQ(nandText.find(i), nandText.end());
	}
}

class MockTestShell : public TestShell {
public:
	MockTestShell(SSDExecutor* reader, SSDExecutor* writer, SSDExecutor* eraser, SSDExecutor* flusher, TestScriptCallback* cb) :
		TestShell{ reader, writer, eraser, flusher, cb } {
	}
	MOCK_METHOD(int, readCompare, (string& expected), (override));

};

TEST_F(TestShellTestFixture, 2_PartialLBAWriteTestWithReadComparePassCondition)
{
	MockTestShell mockShell(&reader, &writer, &eraser, &flusher, &cb);

	EXPECT_CALL(mockShell, readCompare(_))
		.Times(150)
		.WillRepeatedly(Return(0));

	EXPECT_EQ(0, mockShell.run("2_"));

}

TEST_F(TestShellTestFixture, 2_PartialLBAWriteTestWithReadCompareFailCondition)
{
	MockTestShell mockShell(&reader, &writer, &eraser, &flusher, &cb);

	EXPECT_CALL(mockShell, readCompare(_))
		.Times(1)
		.WillRepeatedly(Return(-1));

	EXPECT_EQ(-1, mockShell.run("2_"));

}


TEST_F(TestShellTestFixture, 3_WriteReadAgingValidTest)
{
	MockTestShell mockShell(&reader, &writer, &eraser, &flusher, &cb);

	EXPECT_CALL(mockShell, readCompare(_))
		.Times(400)
		.WillRepeatedly(Return(0));

	EXPECT_EQ(0, mockShell.run("3_"));
}

TEST_F(TestShellTestFixture, 3_WriteReadAgingInValidTest)
{
	int ret = shell->run("3");
	EXPECT_EQ(ret, -1);

	ret = shell->run("3__");
	EXPECT_EQ(ret, -1);
}

TEST_F(TestShellTestFixture, 4_EraseAndWriteAgingValidTest)
{
	int ret = shell->run("4_EraseAndWriteAging");
	EXPECT_EQ(ret, 0);

	ret = shell->run("4_");
	EXPECT_EQ(ret, 0);

	ret = shell->run("4");
	EXPECT_EQ(ret, -1);

	ret = shell->run("4__");
	EXPECT_EQ(ret, -1);
}
