// TestScript.h
#pragma once

#ifdef TESTSCRIPT_EXPORTS
#define TESTSCRIPT_API __declspec(dllexport)
#else
#define TESTSCRIPT_API __declspec(dllimport)
#endif

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "TestScriptCallback.h" 

#define TEST_SCRIPT_DLL_NAME L"TestScript.dll"

class TESTSCRIPT_API TestScript {
public:
    void registerCallback(TestScriptCallback* cb);
    int execute(string command);

private:
    TestScriptCallback* cb_ = nullptr;
    int runFullRead();
    int eraseAndWrite();
    int runSSDEraser(int startLBA, int endLBA, bool print2Console);
    int runSSDWriter(int lba, std::string& data, const int& numOfTimes, bool print2Console);
    void printTestScriptResult(int result);
};

// 함수 선언 (DLL 내보내기)
extern "C" __declspec(dllexport) TestScript* CreateTestScript();
