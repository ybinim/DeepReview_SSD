#include "SSDWriter.h"

#include <iostream>

using namespace std;

int SSDWriter::execute(vector<string>& param, bool print2Console) {
	int ret = checkParam(param);
	if (ret != 0) {
		return ret;
	}

	string command = "ssd.exe W ";
	command += param[1];
	command += " ";
	command += param[2];

	ret = system(command.c_str());
	if (ret != 0) {
		return ret;
	}

	if (print2Console) {
		cout << "[Write] Done" << endl;
	}

	return ret;
}

int SSDWriter::checkParam(vector<string>& param)
{
	if (param.size() != 3) {
		LOG_PRINT("Fail - Invalid parameter size");
		return -2;
	}

	string lba = param[1];
	string data = param[2];

	if (isValidLba(lba) == false) {
		LOG_PRINT("Fail - Invalid LBA format");
		return -2;
	}

	if (data.length() != 10) {
		LOG_PRINT("Fail - Invalid DATA size");
		return -2;
	}

	if (data.substr(0, 2).compare("0x") != 0) {
		LOG_PRINT("Fail - Invalid DATA format");
		return -2;
	}

	for (char& c : data.substr(2, string::npos)) {
		if (!(c >= '0' && c <= '9') && !(c >= 'A' && c <= 'F')) {
			LOG_PRINT("Fail - Invalid DATA format");
			return -2;
		}
	}
	return 0;
}
