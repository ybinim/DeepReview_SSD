#include "TestShell.h"

using namespace std;

int TestShell::run(string command) {
    vector<string> param = parseCommand(command, ' ');
    int result = 0;

    if (param[0].compare("exit") == 0) {
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
            return -2;
        }
        result = runFullRead();
    }
    else if (param[0].compare("fullwrite") == 0) {
        if (param.size() != 2) {
            return -2;
        }
        result = runFullWrite(param);
    }
    else if ((param[0].compare("erase") == 0) || (param[0].compare("erase_range") == 0)) {
        result = eraser->execute(param);
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
        cout << "INVALID COMMAND" << endl;
        return -1;
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
            break;
        }

        fullWriteParam.clear();
    }
    return result;
}

int TestShell::runFullRead(void)
{
    int result = 0;
    vector<string> fullReadParam = {};

    for (int i = 0; i < 100; i++) {
        fullReadParam.push_back("read");
        fullReadParam.push_back(to_string(i));

        result = reader->execute(fullReadParam);
        if (result != 0) {
            break;
        }

        fullReadParam.clear();
    }
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
    std::cout << "  fullwrite [Value]\n";
    std::cout << "  fullread\n";
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
    string expectedData = "0xAAAABBBB";
    vector<string> writeParam;
    vector<string> readParam;
    int result = 0;
    int lba = 0;
    int loopcount = 5;

    while (lba < 100)
    {
        for (int writecount = 0; writecount < loopcount; writecount++)
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

        lba = lba - loopcount;
        for (int comparecount = 0; comparecount < loopcount; comparecount++)
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
    int idx = 0;
    vector<string> lbaList = { "0", "99" };
    vector<string> testParam = {};
    vector<string> dataValue = {};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(0x0, 0xFFFFFFFF);
    unsigned int randValue;

    for (int count = 0; count < 200; count++) {
        idx = 0;
        for (const string& lba : lbaList) {
            std::stringstream ss;
            randValue = dis(gen);
            ss << "0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << randValue;
            //std::cout << "Random Value : " << ss.str() << "\n";

            dataValue.push_back(ss.str());
            testParam.push_back("write");
            testParam.push_back(lba);
            testParam.push_back(dataValue[idx++]);
            result = writer->execute(testParam, print2Console);
            if (result != 0) {
                //break;
                return 1;
            }
            testParam.clear();
        }

        idx = 0;
        for (const string& lba : lbaList) {
            testParam.push_back("read");
            testParam.push_back(lba);

            result = reader->execute(testParam, print2Console);
            if (result != 0) {
                //break;
                return 1;
            }

            result = readCompare(dataValue[idx++]);
            if (result != 0) {
                //break;
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
    file.open("ssd_output.txt");
    if (file.is_open()) {
        string actual = "";
        getline(file, actual);
        if (expected.compare(actual) == 0) {
            return 0;
        }
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
    int result = 0;
    vector<string> eraseParam;

    if (endLBA >= 100) {
        endLBA = 99;
    }
    eraseParam.push_back("erase_range");
    eraseParam.push_back(std::to_string(startLBA));
    eraseParam.push_back(std::to_string(endLBA));
    result = eraser->execute(eraseParam, print2Console);
    return result;
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
