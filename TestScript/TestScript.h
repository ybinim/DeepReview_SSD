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
#include "TestShell.h"

class TestShell;  // 순환 의존성 방지

class TESTSCRIPT_API TestScript {
public:
    //TestScript();
    //~TestScript();
    void addShell(std::shared_ptr<TestShell> shell);
    int execute(string command);
    std::ifstream OpenScriptTxtFile(std::string scriptfilePath);

private:
    std::shared_ptr<TestShell> shell_;
    const std::string scriptfilePath = "ShellScripts.txt";
    int eraseAndWriteAging();
    int runSSDEraser(int startLBA, int endLBA, bool print2Console);
    int runSSDWriter(int lba, std::string& data, const int& numOfTimes, bool print2Console);
    void printTestScriptResult(int result);
};

// 함수 선언 (DLL 내보내기)
extern "C" __declspec(dllexport) TestScript* CreateMyTestScript();
