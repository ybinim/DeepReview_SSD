#include "SSDEraser.h"

#define SSD_ERASE_SIZE_LIMIT (10)

int SSDEraser::execute(vector<string>& param) {
	if (param.size() != 3) {
		return -2;
	}

	int lba = 0;
	int size = 0;

	if (param[0].compare("erase") == 0) {
		lba = stoi(param[1]);
		size = stoi(param[2]);
	}
	else if (param[0].compare("erase") == 0) {
		lba = stoi(param[1]);
		size = stoi(param[2]) - lba + 1;
	}
	return erase(lba, size);
}

int SSDEraser::erase(int lba, int size)
{
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
