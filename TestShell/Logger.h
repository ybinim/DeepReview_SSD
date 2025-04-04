#pragma once
#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

#define LOG_PRINT(str) Logger::getInstance().print(__FUNCTION__, str)

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void print(const string& classFuncName, const string& message);

private:
    Logger() {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static const string logFileName;
    static const size_t MAX_FILE_SIZE;
    ofstream logFile;
};