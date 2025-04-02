#include "write.h"

bool WriteSSD::execute(std::map<int, std::string>& nand, int lba, const std::string& data) {
	myMap = nand;
	auto ret = myMap.emplace(lba, data);

	if (!ret.second) {
		return false;
	}

	if (!updateSSDNandFile()) {
		return false;
	}

	return true;
}

bool WriteSSD::updateSSDNandFile() {
	std::ofstream outFile(PATH_SSD_NAND_FILE);

	if (!outFile.is_open()) {
		return false;
	}

	for (const auto& pair : myMap) {
		outFile << pair.first << " " << pair.second << std::endl;
	}

	outFile.close();
	return true;
}

