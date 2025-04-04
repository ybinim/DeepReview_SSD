#pragma once
#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <filesystem>
#include <sstream>

using namespace std;

#define LOG_PRINT(str) Logger::getInstance().print(__FUNCTION__, str)

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    void openLogFile();
    void closeLogFile();
    void renameOldLogFile();
    void renameOldLogToZip();
    string generateNewFileName();
    bool fileSizeExceedsLimit(const string& fileName);

    void print(const string& classFuncName, const string& message);

private:
    Logger() {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    const string logFolder = "log";
    const string logFileName = "latest.log";
    const string fullFileName = logFolder + "/" + logFileName;
    const size_t MAX_FILE_SIZE = 10 * 1024;  // 10 KB
    ofstream logFile;
};