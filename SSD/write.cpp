#include "write.h"

bool WriteSSD::execute(std::map<int, std::string>& nand, int lba, const std::string& data) {
	auto ret = nand.find(lba);
	if (ret != nand.end()) {
		ret->second = data;
	}
	else {
		nand.emplace(lba, data);
	}

	if (!updateSSDNandFile(nand)) {
		return false;
	}

	return true;
}

bool WriteSSD::updateSSDNandFile(std::map<int, std::string>& nand) {
	std::ofstream outFile(PATH_SSD_NAND_FILE);

	if (!outFile.is_open()) {
		LOG_PRINT("Fail - File is not opened");
		return false;
	}

	for (const auto& pair : nand) {
		outFile << pair.first << " " << pair.second << std::endl;
	}

	outFile.close();
	return true;
}

