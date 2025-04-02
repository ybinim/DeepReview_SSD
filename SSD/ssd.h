#pragma once
#include <string>

enum command {
	Write = 0,
	Read = 1,
};
class SSD {
public:
	bool run(std::string input);
	bool IsInvalidCommand(std::string command);
};