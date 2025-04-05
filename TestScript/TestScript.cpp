// ShellScript.cpp
#include "TestScript.h"

void TestScript::registerCallback(TestScriptCallback* cb) {
    cb_ = cb;
    //std::cout << "[TestScript] Callback registered!" << std::endl;
}

int TestScript::execute(string command) {
    int result = -1;

    //std::cout << command << std::endl;
    if ((command.compare("5_EraseAndWrite2times") == 0) || (command.compare("5_") == 0)) {
        result = eraseAndWrite();
        printTestScriptResult(result);
    }
    else if ((command.compare("6_FullRead") == 0) || (command.compare("6_") == 0)) {
        result = runFullRead();
        printTestScriptResult(result);
    }
    return result;
}

int TestScript::runFullRead(void)
{
    if (!cb_ || !cb_->reader) {
        //std::cout << "[Error] Eraser callback not registered!" << std::endl;
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
        //std::cout << "after runSSDEraser1 : " << result << std::endl;
        return result;
    }

    while (lba + increaseSize < 100) {
        lba += increaseSize;
        int runNumberOfTimes = 2;
        result = runSSDWriter(lba, data, runNumberOfTimes, print2Console);
        if (result != 0) {
            //std::cout << "after runSSDWriter : " << result << std::endl;
            return result;
        }

        result = runSSDEraser(lba, lba + increaseSize, print2Console);
        if (result != 0) {
            //std::cout << "after runSSDEraser2 : " << result << std::endl;
            return result;
        }

    }

    return result;
}

int TestScript::runSSDEraser(int startLBA, int endLBA, bool print2Console)
{
    int result = 0;

    if (!cb_ || !cb_->eraser) {
        //std::cout << "[Error] Eraser callback not registered!" << std::endl;
        return -1;
    }

    if (endLBA >= 100) {
        endLBA = 99;
    }
    //std::cout << startLBA << " " << endLBA << std::endl;
    std::vector<std::string> param = { "erase_range", std::to_string(startLBA), std::to_string(endLBA) };
    return cb_->eraser->execute(param, print2Console);

    return result;
}

int TestScript::runSSDWriter(int lba, std::string& data, const int& numOfTimes, bool print2Console)
{
    int result = 0;

    if (!cb_ || !cb_->writer) {
        //std::cout << "[Error] Writer callback not registered!" << std::endl;
        return -1;
    }

    for (int i = 0; i < numOfTimes; ++i) {
        std::vector<std::string> param = { "write", std::to_string(lba), data };
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
