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

#define TEST_SCRIPT_DLL L"TestScript.dll"
#define TEST_SCRIPT_DLL_NAME_FORMAT L"TestScript_%d.dll"

class TESTSCRIPT_API TestScript {
public:
     void registerCallback(TestScriptCallback* cb);
    int execute(string command);

};

// 함수 선언 (DLL 내보내기)
extern "C" __declspec(dllexport) TestScript* CreateTestScript();
