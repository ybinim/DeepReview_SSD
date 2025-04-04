// ShellScript.cpp
#include "TestScript.h"
//#define TESTSCRIPT_EXPORTS 1
TestScript::TestScript() {
    // 초기화 작업
}

void TestScript::addShell(std::shared_ptr<TestShell> shell) {
    std::cout << "[addShell]" << std::endl;
    shell_ = shell;
}

int TestScript::execute() {
    std::cout << "[Executing Shell Script]" << std::endl;
    //for (const auto& cmd : commands) {
    //    cmd->execute();
    //}

    std::ifstream inputFile = OpenScriptTxtFile("ShellScripts.txt");
    int result = 0;
    string lineStr, cmdStr, keyStr, valueStr;

    // 파일에서 한 줄씩 읽기
    while (std::getline(inputFile, lineStr)) {
        std::cout << lineStr << std::endl;  // 파일의 각 줄을 출력
        stringstream mapStream(lineStr);
        mapStream >> cmdStr >> keyStr >> valueStr;

        vector<string> writeParam;
        writeParam.push_back(cmdStr);
        writeParam.push_back(keyStr);
        writeParam.push_back(valueStr);

        result = shell_->writer->execute(writeParam, false);
        if (result != 0) {
            return result;
        }
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

// DLL에서 내보낼 함수
TestScript* CreateTestScript() {
    return new TestScript();
}