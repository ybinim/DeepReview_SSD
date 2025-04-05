#include "Logger.h"
    
void Logger::openLogFile() {
    if (filesystem::exists(logFolder) == false) {
        // create directory and empty files
        filesystem::create_directory(logFolder);
    }

    if (fileSizeExceedsLimit(fullFileName)) {
        renameOldLogFile();
        compressedOldLog();
        logFile.open(fullFileName, std::ios::app);
    }
    else {
        logFile.open(fullFileName, std::ios::app);
    }

    if (!logFile.is_open()) {
        std::cerr << "Error opening log file!" << std::endl;
    }
}

void Logger::closeLogFile() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::renameOldLogFile() {
    if (filesystem::exists(fullFileName)) {
        string newFileName = logFolder + "/" + generateNewFileName();
        filesystem::rename(fullFileName, newFileName);
    }
}

void Logger::compressedOldLog() {
    vector<filesystem::directory_entry> logFiles;
    for (const auto& entry : filesystem::directory_iterator(logFolder)) {
        if (entry.is_regular_file() && entry.path().filename().string().find("until_") == 0 && entry.path().extension() == ".log") {
            logFiles.push_back(entry);
        }
    }

    if (logFiles.size() > 1) {
        auto oldestFile = *min_element(logFiles.begin(), logFiles.end(),
            [](const filesystem::directory_entry& a, const filesystem::directory_entry& b) {
                return filesystem::last_write_time(a) < filesystem::last_write_time(b);
            });

        filesystem::path oldestFilePath = oldestFile.path();
        filesystem::path tempFilePath = oldestFilePath;
        filesystem::path zipFilePath;
        zipFilePath = tempFilePath.replace_extension(".zip");

        try {
            filesystem::rename(oldestFilePath, zipFilePath);
        }
        catch (const filesystem::filesystem_error& e) {
            std::cerr << "Error compressed file: " << e.what() << std::endl;
        }
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
        << std::setw(2) << std::setfill('0') << timeInfo.tm_min << "m_"
        << std::setw(2) << std::setfill('0') << timeInfo.tm_sec << "s.log";
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
