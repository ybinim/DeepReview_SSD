
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

class Logger {
public:
    Logger() {
        logFile.open(logFileName, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Error opening log file!" << std::endl;
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void print(const string& classFuncName, const string& message) {
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

            /*cout << "[" << timeStr << "] " << classStr << "." << functionStr << std::setw(30) << std::left << "()" 
                << ": " << message << endl;*/
            logFile << "[" << timeStr << "] " << classStr << "." << functionStr << std::setw(30) << std::left << "()"
                 << ": " << message << endl;

        }
    }

private:
    const string logFileName = "latest.txt";
    ofstream logFile;
};

// 매크로를 사용하여 함수명 기록
//#define LOG_FUNCTION(logger) logger.print(__FUNCTION__)

//void sampleFunction1(Logger& logger) {
//    LOG_FUNCTION(logger);
//    // Function logic...
//    std::cout << "Executing sampleFunction1" << std::endl;
//}


//void sampleFunction2(Logger& logger) {
//    LOG_FUNCTION(logger);
//    // Function logic...
//    std::cout << "Executing sampleFunction2" << std::endl;
//}

//int main() {
//    // 로그 파일을 지정하여 로그 기록 클래스 생성
//    Logger logger("function_log.txt");
//
//    // 함수 호출
//    sampleFunction1(logger);
//    sampleFunction2(logger);
//
//    return 0;
//}