#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

#define LOG_PRINT(str) Logger::print(__FUNCTION__, str)

class Logger {
public:
    static void print(const string& classFuncName, const string& message); 

private:
    static ofstream logFile;
    //const string logFileName = "latest.txt";
};