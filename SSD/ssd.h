#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "write.h"
#include "read.h"
#include "logger.h"

class SSD {
public:
	bool run(std::string input);
	bool IsInvalidCommand(std::string command);
	bool IsInvalidLBA(std::string command);
	void ReadFileAndUpdateMap(std::map<int, std::string>& map);

private:
	WriteSSD myWrite;
	ReadSSD myRead;

	const std::string outputfilePath = "ssd_output.txt";
	const std::string nandfilePath = "ssd_nand.txt";
};