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