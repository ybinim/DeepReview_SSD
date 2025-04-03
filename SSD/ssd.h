#pragma once
#include <string>

#include "write.h"
#include "read.h"
#include "erase.h"

class SSD {
public:
	bool run(std::string input);
	bool IsInvalidCommand(std::string command);
	bool IsInvalidLBA(std::string command);
	void ReadFileAndUpdateMap(std::map<int, std::string>& map);

private:
	WriteSSD myWrite;
	ReadSSD myRead;
	EraseSSD myErase;

	const std::string outputfilePath = "ssd_output.txt";
	const std::string nandfilePath = "ssd_nand.txt";
};