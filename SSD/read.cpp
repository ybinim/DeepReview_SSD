#include "read.h"

bool ReadSSD::execute(const std::map<int, std::string>& nand, int lba) {
	std::ofstream outputFile(filePath, std::ios::trunc);
	if (!outputFile.is_open()) {
		return false;
	}

	if (lba < 0 || lba >= 100) {
		outputFile << "ERROR";
		outputFile.close();
		return true;
	}

	auto it = nand.find(lba);
	if (it == nand.end()) {
		outputFile << VALUE_ZERO;
		outputFile.close();
		return true;
	}

	outputFile << it->second;
	outputFile.close();
	return true;
}
