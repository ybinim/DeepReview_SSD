#include "SSDEraser.h"
#include <iostream>

#define SSD_ERASE_SIZE_LIMIT (10)

int SSDEraser::execute(vector<string>& param, bool print2Console) {
	int ret = checkParam(param);
	if (ret != 0) {
		return ret;
	}

	int lba = stoi(param[1]);
	int size;
	if (param[0].compare("erase") == 0) {
		size = stoi(param[2]);
	}
	else {
		size = stoi(param[2]) - stoi(param[1]) + 1;
	}
	
	ret = erase(lba, size);
	if (ret != 0) {
		return ret;
	}

	if (print2Console) {
		std::cout << "[" << param[0] << "] Done" << std::endl;
	}
	return ret;
}

int SSDEraser::checkParam(vector<string>& param)
{
	if (param.size() != 3) {
		LOG_PRINT("Fail - Invalid parameter size");
		return -2;
	}

	if (param[0].compare("erase") == 0) {
		if (isValidLba(param[1]) == false) {
			LOG_PRINT("Fail - Invalid LBA format");
			return -2;
		}
		if (isNumber(param[2]) == false) {
			LOG_PRINT("Fail - Invalid SIZE format");
			return -2;
		}
	}
	else if (param[0].compare("erase_range") == 0) {
		if (isValidLba(param[1]) == false) {
			LOG_PRINT("Fail - Invalid Start_LBA format");
			return -2;
		}
		if (isValidLba(param[2]) == false) {
			LOG_PRINT("Fail - Invalid End_LBA format");
			return -2;
		}

		if ((stoi(param[2]) - stoi(param[1]) + 1) < 0) {
			LOG_PRINT("Fail - Invalid range");
			return -2;
		}
	}
	return 0;
}

int SSDEraser::erase(int lba, int size) {
	int paramSize = 0;
	int ret = 0;
	while ((size > 0) && (lba >= 0) && (lba < 100)) {
		paramSize = (size < SSD_ERASE_SIZE_LIMIT) ? size : SSD_ERASE_SIZE_LIMIT;
		
		string command = "ssd.exe E " + to_string(lba) + " " + to_string(paramSize);
		ret = system(command.c_str());
		if (ret != 0) {
			break;
		}

		size -= paramSize;
		lba += paramSize;
	}

	return ret;
}
