#pragma once
#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>

#include "SSDExecutor.h"
#include "TestScript.h"

using namespace std;

class TestShell
{
public:
    TestShell(SSDExecutor* reader, SSDExecutor* writer, SSDExecutor* eraser, SSDExecutor* flusher)
        : reader(reader), writer(writer), eraser(eraser), flusher(flusher) {}
    int run(string command);
    // TestScript 객체를 등록할 수 있는 함수
    void setTestScript(std::shared_ptr<TestScript> script);
    // TestScript 에서 호출하기 위해 public 으로 변경
    SSDExecutor* reader;
    SSDExecutor* writer;
    SSDExecutor* eraser;
    SSDExecutor* flusher;

private:
    vector<string> parseCommand(string& command, char delimiter);
    int runFullWrite(std::vector<std::string>& param);
    void printHelp(void);
    int runFullRead();
    
    int fullWriteAndReadCompare();
    int partialLBAWrite();
    int writeReadAging();
    virtual int readCompare(string& expected);
    void printTestScriptResult(int result);
    int eraseAndWriteAging();
    int runSSDEraser(int startLBA, const int endLBA, bool print2Console);
    int runSSDWriter(int lba, std::string& data, const int& numOfTimes, bool print2Console);
    std::shared_ptr<TestScript> script_;  // TestScript 객체를 저장할 변수
};