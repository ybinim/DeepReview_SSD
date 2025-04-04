#include "Logger.h"

const string Logger::logFileName = "latest.log";
const size_t Logger::MAX_FILE_SIZE = 10*1024;  // 10 KB

void Logger::print(const string& classFuncName, const string& message) {
    time_t now = time(nullptr);
    struct tm timeInfo;
    char timeStr[100];

    logFile.open(logFileName, std::ios::app);
    if (logFile.is_open()) {
        localtime_s(&timeInfo, &now);
        strftime(timeStr, sizeof(timeStr), "%y.%m.%d %H:%M", &timeInfo);

        string classStr, functionStr;
        size_t pos = classFuncName.find("::");
        if (pos != std::string::npos) {
            classStr = classFuncName.substr(0, pos);
            functionStr = classFuncName.substr(pos + 2);
        }
        
        string fullFunctionName = classStr + "." + functionStr + "()";

        cout << "[" << timeStr << "] "
            << std::setw(30) << std::left << fullFunctionName
            << ": " << message << endl;

        logFile << "[" << timeStr << "] "
            << std::setw(30) << std::left << fullFunctionName
            << ": " << message << endl;
    }
    logFile.close();
}
