#include "logger.h"

ofstream Logger::logFile;
const string Logger::logFileName = "latest.log";
const size_t Logger::MAX_FILE_SIZE = 10240;  // 10 KB

void Logger::openLogFile() {
    if (!logFile.is_open()) {
        if (fileSizeExceedsLimit(logFileName)) {
            renameOldLogFile();  
            logFile.open(logFileName, std::ios::app);
        }
        else {
            logFile.open(logFileName, std::ios::app);
        }

        if (!logFile.is_open()) {
            std::cerr << "Error opening log file!" << std::endl;
        }
    }
}

void Logger::closeLogFile() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::renameOldLogFile() {
    string newFileName = generateNewFileName();
    if (std::filesystem::exists(logFileName)) {
        std::filesystem::rename(logFileName, newFileName);
    }
}

string Logger::generateNewFileName() {
    time_t now = time(nullptr);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);
    stringstream ss;

    ss << "until_"
        << std::setw(2) << std::setfill('0') << (timeInfo.tm_year + 1900) % 100
        << std::setw(2) << std::setfill('0') << (timeInfo.tm_mon + 1)
        << std::setw(2) << std::setfill('0') << timeInfo.tm_mday << "_"
        << std::setw(2) << std::setfill('0') << timeInfo.tm_hour << "h_"
        << std::setw(2) << std::setfill('0') << timeInfo.tm_min  << "m_"
        << std::setw(2) << std::setfill('0') << timeInfo.tm_sec  << "s.log";
    return ss.str();
}

bool Logger::fileSizeExceedsLimit(const string& fileName) {
    ifstream file(fileName, ios::binary | ios::ate);
    return file.is_open() && file.tellg() > MAX_FILE_SIZE;
}

void Logger::print(const string& classFuncName, const string& message) {
    openLogFile();

    if (logFile.is_open()) {
        time_t now = time(nullptr);
        struct tm timeInfo;
        char timeStr[100];

        localtime_s(&timeInfo, &now);
        strftime(timeStr, sizeof(timeStr), "%y.%m.%d %H:%M", &timeInfo);

        string classStr, functionStr;
        size_t pos = classFuncName.find("::");
        if (pos != std::string::npos) {
            classStr = classFuncName.substr(0, pos);
            functionStr = classFuncName.substr(pos + 2);
        }

        string fullFunctionName = classStr + "." + functionStr + "()";

        /*cout << "[" << timeStr << "] "
            << std::setw(30) << std::left << fullFunctionName
            << ": " << message << endl;*/

        logFile << "[" << timeStr << "] "
            << std::setw(30) << std::left << fullFunctionName
            << ": " << message << endl;

        closeLogFile();
    }
}