#pragma once
#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>

#include "SSDExecutor.h"

using namespace std;

class TestShell
{
public:
    TestShell(SSDExecutor* reader, SSDExecutor* writer, SSDExecutor* eraser)
        : reader(reader), writer(writer), eraser(eraser) {}
    int run(string command);

private:
    vector<string> parseCommand(string& command, char delimiter);
    int runFullWrite(std::vector<std::string>& param);
    void printHelp(void);
    int runFullRead();
    
    int fullWriteAndReadCompare();
    int partialLBAWrite();
    int writeReadAging();
    virtual int readCompare(string& expected);
    void printTestScriptResult(int result);
  
    SSDExecutor* reader;
    SSDExecutor* writer;
    SSDExecutor* eraser;
};