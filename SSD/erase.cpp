#include "erase.h"

bool EraseSSD::execute(std::map<int, std::string>& nand, int lba, const std::string& size) {
	bool ret = false;
	if (ret = GetSizeAfterCheckInvalidSizeOrNot(size, &eraseSize))
	{
		std::ofstream outputFile(outputfilePath, std::ios::trunc);
		outputFile << "ERROR";
		return false;
	}
	
	if (eraseSize != 0)
	{
		for (int i = 0; i < eraseSize; i++)
		{
			if (lba + i >= 100) break;
			auto ret = nand.find(lba+i);
			if (ret != nand.end()) {
				nand.erase(lba+i);
			}
		}

		if (!updateSSDNandFile(nand)) {
			return false;
		}
	}

	return true;
}

bool EraseSSD::GetSizeAfterCheckInvalidSizeOrNot(const std::string& sizeStr, int* getValidSize)
{
	int size = 0;
	bool ret = false;

	try {
		size = std::stoi(sizeStr);

		// 0은 에러가 아니라고 했음.
		if (size < 0 || size > 10) {
			ret = true;
		}
	}
	catch (const std::invalid_argument& e) {
		ret = true;
	}
	catch (const std::out_of_range& e) {
		ret = true;
	}

	*getValidSize = size;
	return ret;
}

bool EraseSSD::updateSSDNandFile(std::map<int, std::string>& nand) {
	std::ofstream outFile(nandfilePath);

	if (!outFile.is_open()) {
		return false;
	}

	for (const auto& pair : nand) {
		outFile << pair.first << " " << pair.second << std::endl;
	}

	outFile.close();
	return true;
}
