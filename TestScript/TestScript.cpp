// ShellScript.cpp
#include "TestScript.h"

#define SSD_OUTPUT_FILEPATH ("ssd_output.txt")

void TestScript::registerCallback(TestScriptCallback* cb) {
    cb_ = cb;
}

int TestScript::execute(string command) {
    int result = -1;
#if 0
    if ((command.compare("1_FullWriteAndReadCompare") == 0) || (command.compare("1_") == 0)) {
            result = fullWriteAndReadCompare();
            printTestScriptResult(result);
    }
#endif
    if ((command.compare("5_FullRead") == 0) || (command.compare("5_") == 0)) {
        result = runFullRead();
        printTestScriptResult(result);
    }
#if 0
    elseif ((command.compare("6_EraseAndWrite2times") == 0) || (command.compare("6_") == 0)) {
        result = eraseAndWrite();
        printTestScriptResult(result);
    }
#endif
    return result;
}

int TestScript::fullWriteAndReadCompare() {
    bool print2Console = false;
    int data = 53681003;
    string expectedData = "0x";
    vector<string> writeParam;
    vector<string> readParam;
    int result = 0;
    int lba = 0;
    const int loopSize = 5;

    while (lba < 100)
    {
        expectedData += to_string(data);
        data += 16;
        for (int writecount = 0; writecount < loopSize; writecount++)
        {
            writeParam.push_back("write");
            writeParam.push_back(to_string(lba++));
            writeParam.push_back(expectedData);

            result = cb_->writer->execute(writeParam, print2Console);
            if (result != 0) {
                return result;
            }
            writeParam.clear();
        }

        lba = lba - loopSize;
        for (int comparecount = 0; comparecount < loopSize; comparecount++)
        {
            readParam.push_back("read");
            readParam.push_back(to_string(lba++));

            result = cb_->reader->execute(readParam, print2Console);
            if (result != 0) {
                return result;
            }
            readParam.clear();

            result = readCompare(expectedData);
            if (result != 0) {
                return result;
            }
        }
        expectedData = "0x";
    }

    return result;
}

int TestScript::readCompare(string& expected) {
    ifstream file;
    file.open(SSD_OUTPUT_FILEPATH);
    if (file.is_open()) {
        string actual = "";
        getline(file, actual);
        if (expected.compare(actual) == 0) {
            return 0;
        }
        file.close();
    }
    return -1;
}

int TestScript::runFullRead(void)
{
    if (!cb_ || !cb_->reader) {
        std::cout << "runFullRead !cb_\n";
        return -1;
    }

    bool print2console = false;
    int result = 0;
    vector<string> fullReadParam = { "read", "-1" };
    string data = "";

    for (int i = 0; i < 100; i++) {
        fullReadParam[1] = to_string(i);

        result = cb_->reader->execute(fullReadParam, print2console);
        if (result != 0) {
            return result;
        }
    }
    std::cout << "[FullRead] Done" << endl;
    return result;
}

int TestScript::eraseAndWrite() {
    bool print2Console = false;
    string data = "0x1234ABCD";
    int result = 0;
    int lba = 0;
    const int increaseSize = 2;

    result = runSSDEraser(lba, lba + increaseSize, print2Console);
    if (result != 0) {
        return result;
    }

    while (lba + increaseSize < 100) {
        lba += increaseSize;
        int runNumberOfTimes = 2;
        result = runSSDWriter(lba, data, runNumberOfTimes, print2Console);
        if (result != 0) {
            return result;
        }

        result = runSSDEraser(lba, lba + increaseSize, print2Console);
        if (result != 0) {
            return result;
        }

    }

    return result;
}

int TestScript::runSSDEraser(int startLBA, int endLBA, bool print2Console)
{
    int result = 0;

    if (!cb_ || !cb_->eraser) {
        return -1;
    }

    if (endLBA >= 100) {
        endLBA = 99;
    }
    vector<string> param = { "erase_range", std::to_string(startLBA), std::to_string(endLBA) };
    return cb_->eraser->execute(param, print2Console);

    return result;
}

int TestScript::runSSDWriter(int lba, std::string& data, const int& numOfTimes, bool print2Console)
{
    int result = 0;

    if (!cb_ || !cb_->writer) {
        return -1;
    }

    for (int i = 0; i < numOfTimes; ++i) {
        vector<string> param = { "write", std::to_string(lba), data };
        int result = cb_->writer->execute(param, print2Console);
        if (result != 0) return result;
    }
    return result;
}

void TestScript::printTestScriptResult(int result) {
    if (result == 0) {
        cout << "PASS" << endl;
    }
    else {
        cout << "FAIL" << endl;
    }
}

// DLL에서 내보낼 함수 정의
extern "C" __declspec(dllexport) TestScript* CreateTestScript() {
    return new TestScript();  // TestScript 객체를 동적으로 생성하여 반환
}
