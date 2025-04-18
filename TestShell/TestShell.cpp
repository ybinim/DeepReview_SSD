﻿#include <windows.h>   // DLLMain 관련 헤더
#include "TestShell.h"

#define SSD_OUTPUT_FILEPATH ("ssd_output.txt")

using namespace std;

typedef TestScript* (*CreateTestScriptFunc)(); // DLL에서 TestScript 객체 생성 함수

TestShell::TestShell(SSDExecutor* reader, SSDExecutor* writer, SSDExecutor* eraser, SSDExecutor* flusher)
    : reader(reader), writer(writer), eraser(eraser), flusher(flusher) {
    // 2. TestScript에 등록할 콜백 구조체 세팅
    cb.reader = reader; cb.writer = writer; cb.eraser = eraser; cb.flusher = flusher;
}

int TestShell::run(string command) {
    vector<string> param = parseCommand(command, ' ');
    int result = 0;
    
    if (command.empty()) {
        return 0;
    }

    if (param[0].compare("exit") == 0) {
        LOG_PRINT("Exit");
        return 1;
    }
    else if (param[0].compare("read") == 0) {
        result = reader->execute(param);
    }
    else if (param[0].compare("write") == 0) {
        result = writer->execute(param);
    }
    else if (param[0].compare("fullread") == 0) {
        if (param.size() != 1) {
            LOG_PRINT("Fail - Invalid parameter size");
            return -2;
        }
        result = runFullRead();
    }
    else if (param[0].compare("fullwrite") == 0) {
        if (param.size() != 2) {
            LOG_PRINT("Fail - Invalid parameter size");
            return -2;
        }
        result = runFullWrite(param);
    }
    else if ((param[0].compare("erase") == 0) || (param[0].compare("erase_range") == 0)) {
        result = eraser->execute(param);
    }
    else if (param[0].compare("flush") == 0) {
        result = flusher->execute(param);
    }
    else if (param[0].compare("help") == 0) {
        printHelp();
    }
    else if ((param[0].compare("1_FullWriteAndReadCompare") == 0) || (param[0].compare("1_") == 0)) {
        result = fullWriteAndReadCompare();
        printTestScriptResult(result);
    }
    else if ((param[0].compare("2_PartialLBAWrite") == 0) || (param[0].compare("2_") == 0)) {
        result = partialLBAWrite();
        printTestScriptResult(result);
    }
    else if ((param[0].compare("3_WriteReadAging") == 0) || (param[0].compare("3_") == 0)) {
        result = writeReadAging();
        printTestScriptResult(result);
    }
    else if ((param[0].compare("4_EraseAndWriteAging") == 0) || (param[0].compare("4_") == 0)) {
        result = eraseAndWriteAging();
        printTestScriptResult(result);
    }
    else {
        result = runTestScript(command);
        if (result != 0) {
            LOG_PRINT("Fail - Invalid command (" + command + ")");
            cout << "INVALID COMMAND" << endl;
            return -1;
        }
    }
    return result;
}

int TestShell::runFullWrite(std::vector<std::string>& param)
{
    bool print2console = false;
    int result = 0;
    vector<string> fullWriteParam = {};

    string data = param[1];
    for (int i = 0; i < 100; i++) {
        fullWriteParam.push_back("write");
        fullWriteParam.push_back(to_string(i));
        fullWriteParam.push_back(data);

        result = writer->execute(fullWriteParam, print2console);
        if (result != 0) {
            return result;
        }

        fullWriteParam.clear();
    }
    cout << "[FullWrite] Done" << endl;
    return result;
}

int TestShell::runFullRead(void)
{
    int result = 0;
    vector<string> fullReadParam = { "read", "-1"};
    string data = "";

    for (int i = 0; i < 100; i++) {
        fullReadParam[1] = to_string(i);

        result = reader->execute(fullReadParam);
        if (result != 0) {
            return result;
        }
    }
    cout << "[FullRead] Done" << endl;
    return result;
}

vector<string> TestShell::parseCommand(string& command, char delimiter) {
    string subParam = "";
    vector<string> result = {};
    istringstream commandStream(command);

    while (getline(commandStream, subParam, delimiter)) {
        result.push_back(subParam);
    }

    return result;
}

void TestShell::printTestScriptResult(int result) {
    if (result == 0) {
        cout << "PASS" << endl;
    }
    else {
        cout << "FAIL" << endl;
    }
}

void TestShell::printHelp() {
    // Title Output
    std::cout << "=====================================\n";
    std::cout << "              Program Help\n";
    std::cout << "=====================================\n\n";

    // Author Information
    std::cout << "Author  : DeepReviewer / 윤이나, 이용재, 임용빈, 장영진\n\n";

    // Usage
    std::cout << "Usage   : Command [arguments] [arguments]\n\n";

    // Commands
    std::cout << "Commands :\n";
    std::cout << "  write [LBA] [DATA]\n";
    std::cout << "  read [LBA]\n";
    std::cout << "  fullwrite [DATA]\n";
    std::cout << "  fullread\n";
    std::cout << "  erase [LBA] [SIZE]\n";
    std::cout << "  erase_range [Start LBA] [End LBA]\n";
    std::cout << "  flush\n";
    std::cout << "  exit\n";

    // Examples
    std::cout << "\nExamples:\n";
    std::cout << std::setw(30) << std::left << "  write 3 0xAAAABBBB"
        << "  : 3번 LBA에 0xAAAABBBB를 기록\n";
    std::cout << std::setw(30) << std::left << "  read 3"
        << "  : 3번 LBA를 읽는다\n";
    std::cout << std::setw(30) << std::left << "  fullwrite 0xABCDFFFF"
        << "  : 전체 LBA에 0xABCDFFFF를 기록\n";
    std::cout << std::setw(30) << std::left << "  fullread"
        << "  : 전체 LBA의 값을 화면에 출력\n";
    std::cout << std::setw(30) << std::left << "  exit"
        << "  : 프로그램 종료\n";
    // Notes
    std::cout << "\nNote:\n";
    std::cout << "  [LBA]   : 0~99, 10진수로 입력받음\n";
    std::cout << "  [Value] : 0x00000000~0xFFFFFFFF, 0x가 붙으며 10 글자로 표기\n";

    // End of Help
    std::cout << "\n=====================================\n";
    std::cout << "              End of Help\n";
    std::cout << "=====================================\n";
}

int TestShell::fullWriteAndReadCompare() {
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

            result = writer->execute(writeParam, print2Console);
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

            result = reader->execute(readParam, print2Console);
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

int TestShell::partialLBAWrite() {
    bool print2Console = false;
    string data = "0x11223344";
    vector<string> lbaList = { "4", "0", "3", "1", "2"};
    vector<string> writeParam;
    vector<string> readParam;
    int result = 0;

    for (int count = 0; count < 30; count++) {
        for (const string& lba : lbaList) {
            writeParam.push_back("write");
            writeParam.push_back(lba);
            writeParam.push_back(data);

            result = writer->execute(writeParam, print2Console);
            if (result != 0) {
                return result;
            }
            writeParam.clear();
        }

        for (const string& lba : lbaList) {
            readParam.push_back("read");
            readParam.push_back(lba);

            result = reader->execute(readParam, print2Console);
            if (result != 0) {
                return result;
            }
            
            result = readCompare(data);
            if (result != 0) {
                return result;
            }
            readParam.clear();
        }
    }

    return result;
}

int TestShell::writeReadAging() {
    bool print2Console = false;
    int result = 0;
    vector<string> lbaList = { "0", "99" };
    vector<string> testParam = {};
    vector<string> dataValue = {};
    int dataValueIdx = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(0x0, 0xFFFFFFFF);
    unsigned int randValue;

    for (int count = 0; count < 200; count++) {
        dataValueIdx = 0;
        for (const string& lba : lbaList) {
            std::stringstream ss;
            randValue = dis(gen);
            ss << "0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << randValue;

            dataValue.push_back(ss.str());
            testParam.push_back("write");
            testParam.push_back(lba);
            testParam.push_back(dataValue[dataValueIdx++]);
            result = writer->execute(testParam, print2Console);
            if (result != 0) {
                return 1;
            }
            testParam.clear();
        }

        dataValueIdx = 0;
        for (const string& lba : lbaList) {
            testParam.push_back("read");
            testParam.push_back(lba);

            result = reader->execute(testParam, print2Console);
            if (result != 0) {
                return 1;
            }

            result = readCompare(dataValue[dataValueIdx++]);
            if (result != 0) {
                return 1;
            }
            testParam.clear();
        }
        dataValue.clear();
    }

    return 0;
}

int TestShell::readCompare(string& expected) {
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

int TestShell::eraseAndWriteAging() {
    bool print2Console = false;
    string data = "0x1234ABCD";
    int result = 0;
    int lba = 0;
    const int increaseSize = 2;

    result = runSSDEraser(lba, lba+increaseSize, print2Console);
    if (result != 0) {
        return result;
    }

    for (int count = 0; count < 30; count++) {
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
        lba = 0;
    }

    return result;
}

int TestShell::runSSDEraser(int startLBA, int endLBA, bool print2Console)
{
    vector<string> eraseParam;

    if (endLBA >= 100) {
        endLBA = 99;
    }
    eraseParam.push_back("erase_range");
    eraseParam.push_back(std::to_string(startLBA));
    eraseParam.push_back(std::to_string(endLBA));
    return eraser->execute(eraseParam, print2Console);
}

int TestShell::runSSDWriter(int lba, std::string& data, const int& numOfTimes, bool print2Console)
{
    int result = 0;
    for (int writeCnt = 0; writeCnt < numOfTimes; writeCnt++) {
        vector<string> writeParam;

        writeParam.push_back("write");
        writeParam.push_back(std::to_string(lba));
        writeParam.push_back(data);

        result = writer->execute(writeParam, print2Console);
        if (result != 0) {
            return result;
        }
    }
    return result;
}

void TestShell::setTestScript(std::shared_ptr<TestScript> script) {
    script_ = script;  // TestScript 객체를 저장
}

int TestShell::runTestScript(string command) {
    int result = -1;

    result = loadDLLAndRegisterCallback(TEST_SCRIPT_DLL);
    if (result == 0) {
        result = script_->execute(command);
        if (result == 0) {
            LOG_PRINT("The command execution was successful!");
            return result;
        }
    }

    // If TEST_SCRIPT_DLL failed, try TestScript_0.dll to TestScript_9.dll
    for (int i = 0; i < 10; ++i) {
        wchar_t dllName[MAX_PATH];
        swprintf(dllName, MAX_PATH, TEST_SCRIPT_DLL_NAME_FORMAT, i);

        result = loadDLLAndRegisterCallback(dllName);
        if (result == 0) {
            result = script_->execute(command);
            if (result == 0) {
                LOG_PRINT("The command execution was successful!");
                return result;
            }
        }
    }

    return result;
}

int TestShell::loadDLLAndRegisterCallback(const wchar_t* dllName) {
    HMODULE hDLL = LoadLibrary(dllName);
    if (!hDLL) {
        LOG_PRINT("Unable to load DLL.");
        return -1;
    }

    // Try to load the function from the DLL
    CreateTestScriptFunc createTestScript = (CreateTestScriptFunc)GetProcAddress(hDLL, "CreateTestScript");
    if (!createTestScript) {
        LOG_PRINT("Unable to find CreateTestScript function");
        FreeLibrary(hDLL);
        return -1;
    }

    // Create TestScript object
    TestScript* script = createTestScript();
    if (!script) {
        LOG_PRINT("Failed to create TestScript object.");
        FreeLibrary(hDLL);
        return -1;
    }

    // Register the TestScript object and callback
    script_ = std::shared_ptr<TestScript>(script);
    script_->registerCallback(&cb);

    // Unload the DLL after registration
    //FreeLibrary(hDLL);

    return 0;
}
