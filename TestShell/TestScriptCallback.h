#pragma once
#include "SSDExecutor.h"

struct TestScriptCallback {
    SSDExecutor* reader;
    SSDExecutor* writer;
    SSDExecutor* eraser;
    SSDExecutor* flusher;
};
