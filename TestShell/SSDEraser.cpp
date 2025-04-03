#include "SSDEraser.h"
#include <iostream>

#define SSD_ERASE_SIZE_LIMIT (10)

int SSDEraser::execute(vector<string>& param, bool print2Console) {
	if (param.size() != 3) {
		return -2;
	}

	int lba = 0;
	int size = 0;

	if (param[0].compare("erase") == 0) {
		if (param[1].length() > 2 || isNumber(param[1]) == false) {
			return -2;
		}
		if (isNumber(param[2]) == false) {
			return -2;
		}

		lba = stoi(param[1]);
		size = stoi(param[2]);
	}
	else if (param[0].compare("erase_range") == 0) {
		if (param[1].length() > 2 || isNumber(param[1]) == false) {
			return -2;
		}
		if (param[2].length() > 2 || isNumber(param[2]) == false) {
			return -2;
		}

		lba = stoi(param[1]);
		size = stoi(param[2]) - lba + 1;
		if (size < 0) {
			return -2;
		}
	}
	
	int ret = erase(lba, size);
	if (print2Console) {
		std::cout << "[" << param[0] << "] Done" << std::endl;
	}
	return ret;
}

int SSDEraser::erase(int lba, int size) {
	int paramSize = 0;
	int ret = 0;
	while ((size > 0) && (lba >= 0) && (lba < 100)) {
		paramSize = (size < SSD_ERASE_SIZE_LIMIT) ? size : SSD_ERASE_SIZE_LIMIT;
		
		string command = "E " + to_string(lba) + " " + to_string(paramSize);
		ret = system(command.c_str());
		if (ret != 0) {
			break;
		}

		size -= paramSize;
		lba += paramSize;
	}

	return ret;
}
