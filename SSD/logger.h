#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <filesystem>

using namespace std;

#define LOG_PRINT(str) Logger::print(__FUNCTION__, str)

class Logger {
public:
    static void print(const string& classFuncName, const string& message);
    static void openLogFile();
    static bool fileSizeExceedsLimit(const string& fileName);
    static string generateNewFileName();
    static void renameOldLogFile();
    static void closeLogFile();

private:
    static ofstream logFile;
    static const string logFileName;   // 로그 파일 이름
    static const size_t MAX_FILE_SIZE; // 파일 최대 크기 (10 KB)
};