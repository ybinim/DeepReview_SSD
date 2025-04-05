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

class TESTSCRIPT_API TestScript {
public:
     void registerCallback(TestScriptCallback* cb);
    int execute(string command);

};
