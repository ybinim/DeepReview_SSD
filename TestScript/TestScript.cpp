// ShellScript.cpp
#include "TestScript.h"

void TestScript::addShell(std::shared_ptr<TestShell> shell) {
    std::cout << "[addShell]" << std::endl;
    shell_ = shell;
}

int TestScript::execute(string command) {
    std::cout << "[Executing Shell Script]" << std::endl;
 
    std::cout << command << std::endl;
}

int TestScript::eraseAndWriteAging() {
    bool print2Console = false;
    string data = "0x1234ABCD";
    int result = 0;
    int lba = 0;
    const int increaseSize = 2;

    result = runSSDEraser(lba, lba + increaseSize, print2Console);
    if (result != 0) {
        return result;
    }

    for (int count = 0; count < 2; count++) {
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

int TestScript::runSSDEraser(int startLBA, int endLBA, bool print2Console)
{
    int result = 0;
    vector<string> eraseParam;

    if (endLBA >= 100) {
        endLBA = 99;
    }
    eraseParam.push_back("erase_range");
    eraseParam.push_back(std::to_string(startLBA));
    eraseParam.push_back(std::to_string(endLBA));
    result = shell_->eraser->execute(eraseParam, print2Console);
    return result;
}

int TestScript::runSSDWriter(int lba, std::string& data, const int& numOfTimes, bool print2Console)
{
    int result = 0;
    for (int writeCnt = 0; writeCnt < numOfTimes; writeCnt++) {
        vector<string> writeParam;

        writeParam.push_back("write");
        writeParam.push_back(std::to_string(lba));
        writeParam.push_back(data);

        result = shell_->writer->execute(writeParam, print2Console);
        if (result != 0) {
            return result;
        }
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

std::ifstream TestScript::OpenScriptTxtFile(std::string scriptfilePath) {
    // 파일을 읽기 모드로 연다
    std::ifstream inputFile(scriptfilePath);

    // 파일이 열렸는지 확인
    if (!inputFile) {
        std::cerr << "파일을 열 수 없습니다!" << std::endl;
        //return;  // 오류 발생 시 종료
    }

    return inputFile;

    // 파일 닫기
 //   inputFile.close();
}

// DLL에서 내보낼 함수 정의
extern "C" __declspec(dllexport) TestScript* CreateMyTestScript() {
    return new TestScript();  // TestScript 객체를 동적으로 생성하여 반환
}
