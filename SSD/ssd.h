#pragma once
#include <string>

#include "SSD_Read.cpp"
#include "write.cpp"

enum command {
	Write = 0,
	Read = 1,
};
class SSD {
public:
	bool run(std::string input);
	bool IsInvalidCommand(std::string command);

private:
	WriteSSD myWrite;
	ReadSSD myRead;
};