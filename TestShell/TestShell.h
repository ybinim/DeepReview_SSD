#pragma once
#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "SSDWriter.h"

using namespace std;

class TestShell
{
public:
    int run(string command);

private:
    vector<string> parseCommand(string& command, char delimiter);

    SSDWriter writer;
};