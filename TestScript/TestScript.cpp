// ShellScript.cpp
#include "TestScript.h"

#define SSD_OUTPUT_FILEPATH ("ssd_output.txt")

void TestScript::registerCallback(TestScriptCallback* cb) {
    cb_ = cb;
}

int TestScript::execute(string command) {
    int result = -1;
    // implement your testscript here.
    return result;
}

// DLL에서 내보낼 함수 정의
extern "C" __declspec(dllexport) TestScript* CreateTestScript() {
    return new TestScript();  // TestScript 객체를 동적으로 생성하여 반환
}
