#include "read.h"

bool ReadSSD::execute(const std::map<int, std::string>& nand, int lba) {
	std::ofstream outputFile(outputFilePath, std::ios::trunc);
	if (!outputFile.is_open()) {
		return false;
	}

	auto it = nand.find(lba);
	if (it == nand.end()) {
		outputFile << zeroValue;
	}
	else {
		outputFile << it->second;
	}

	outputFile.close();
	return true;
}
