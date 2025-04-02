#include "read.h"

bool ReadSSD::execute(const std::map<int, std::string>& nand, int lba) {
	std::ofstream outputFile(filePath, std::ios::trunc);
	if (!outputFile.is_open()) {
		return false;
	}

	auto it = nand.find(lba);
	if (it == nand.end()) {
		outputFile << VALUE_ZERO;
	}
	else {
		outputFile << it->second;
	}

	outputFile.close();
	return true;
}
