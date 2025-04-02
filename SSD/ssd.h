#pragma once
#include <string>

#include "write.h"
#include "SSD_Read.cpp"

class SSD {
public:
	bool run(std::string input);
	bool IsInvalidCommand(std::string command);

private:
	WriteSSD myWrite;
	ReadSSD myRead;
};