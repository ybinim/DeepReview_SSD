#pragma once
#include "SSDExecutor.h"

/*
Here's an example of how to call the functions.

bool print2Console = false;

vector<string> readParam = { "read", "0" };
cb_->reader->execute(readParam, print2console);

vector<string> writeParam = { "write", "0", 0x1234ABCD };
cb_->writer->execute(writeParam, print2Console);

vector<string> eraseParam1 = { "erase", "0", "10" };
cb_->eraser->execute(eraseParam1, print2Console);
vector<string> eraseParam2 = { "erase_range", "0", "99" };
cb_->eraser->execute(eraseParam2, print2Console);

vector<string> flushParam = { "flush" };
cb_->flusher->execute(flushParam, print2Console);
*/
struct TestScriptCallback {
    SSDExecutor* reader;
    SSDExecutor* writer;
    SSDExecutor* eraser;
    SSDExecutor* flusher;
};
