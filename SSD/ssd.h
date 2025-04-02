#pragma once
#include <string>

#include "write.h"
#include "read.h"

class SSD {
public:
	bool run(std::string input);
	bool IsInvalidCommand(std::string command);

private:
	WriteSSD myWrite;
	ReadSSD myRead;
};