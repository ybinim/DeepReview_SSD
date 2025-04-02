#pragma once
#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include "SSDRunner.h"

using namespace std;

class TestShell
{
public:
    TestShell(SSDRunner* reader, SSDRunner* writer) : reader(reader), writer(writer) {}
    int run(string command);

private:
    vector<string> parseCommand(string& command, char delimiter);
    int runFullWrite(std::vector<std::string>& param);
    void printHelp(void);

    int fullWriteAndReadCompare();
    int partialLBAWrite();
    int writeReadAging();
    void printTestScriptResult(int result);
    int readCompare(string& expected);

    SSDRunner* reader;
    SSDRunner* writer;
};