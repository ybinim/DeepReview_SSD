// ShellScript.cpp
#include "TestScript.h"

#define SSD_OUTPUT_FILEPATH ("ssd_output.txt")

void TestScript::registerCallback(TestScriptCallback* cb) {
    cb_ = cb;
}

int TestScript::execute(string command) {
    int result = -1;
    // implement your testscript here.
    if ((command.compare("5_FullRead") == 0) || (command.compare("5_") == 0)) {
        result = runFullRead();
    }
    return result;
}

int TestScript::runFullRead(void)
{
    int result = 0;
    vector<string> fullReadParam = { "read", "-1" };
    string data = "";

    for (int i = 0; i < 100; i++) {
        fullReadParam[1] = to_string(i);

        result = cb_->reader->execute(fullReadParam);
        if (result != 0) {
            return result;
        }
    }
    cout << "[FullRead] Done" << endl;
    return result;
}

// DLL에서 내보낼 함수 정의
extern "C" __declspec(dllexport) TestScript* CreateTestScript() {
    return new TestScript();  // TestScript 객체를 동적으로 생성하여 반환
}
