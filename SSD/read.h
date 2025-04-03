#ifndef READSDD_H
#define READSDD_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>

#include "logger.h"

class ReadSSD {
public:
    bool execute(const std::map<int, std::string>& nand, int lba);

private:
	const std::string filePath = "ssd_output.txt";
	const std::string VALUE_ZERO = "0x00000000";
};

#endif