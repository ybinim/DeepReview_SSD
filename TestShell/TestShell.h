#pragma once
#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "SSDRunner.h"

using namespace std;

class TestShell
{
public:
    TestShell(SSDRunner* reader, SSDRunner* writer) : reader(reader), writer(writer) {}
    int run(string command);

private:
    vector<string> parseCommand(string& command, char delimiter);
    void printHelp(void);

    SSDRunner* reader;
    SSDRunner* writer;
};