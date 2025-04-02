#pragma once
#include <string>

#include "write.h"
#include "read.h"

class SSD {
public:
	bool run(std::string input);
	bool IsInvalidCommand(std::string command);
	bool IsInvalidLBA(std::string command);

private:
	WriteSSD myWrite;
	ReadSSD myRead;

	const std::string outputfilePath = "ssd_output.txt";
	const std::string nandfilePath = "ssd_nand.txt";
};