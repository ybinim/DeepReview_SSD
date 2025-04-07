#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>

class ReadSSD {
public:
    bool execute(const std::map<int, std::string>& nand, int lba);

private:
	const std::string outputFilePath = "ssd_output.txt";
	const std::string zeroValue = "0x00000000";
};
