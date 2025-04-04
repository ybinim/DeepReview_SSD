#include <string>
#include <iomanip>
#include <fstream>
#include <iostream>

using namespace std;

class Logger {
public:
    const string logFileName = "latest.log";
    ofstream logFile;

    void print(const string& classFuncName, const string& message) {
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

        cout << "[" << timeStr << "] " << classStr << "." << functionStr << std::setw(30) << std::left << "()"
            << ": " << message << endl;
     
    }
};