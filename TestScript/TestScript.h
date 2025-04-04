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

class TESTSCRIPT_API TestScript {
public:
    TestScript();
    void addShell(std::shared_ptr<TestShell> shell);
    int execute();
    std::ifstream OpenScriptTxtFile(std::string scriptfilePath);

private:
    std::shared_ptr<TestShell> shell_;
    const std::string scriptfilePath = "ShellScripts.txt";
};

// DLL에서 TestScript 객체 생성 함수
extern "C" TESTSCRIPT_API TestScript* createTestScript();
