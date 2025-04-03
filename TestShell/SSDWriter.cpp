#include "SSDWriter.h"

#include <iostream>

using namespace std;

int SSDWriter::execute(vector<string>& param, bool print2Console) {
	if (param.size() != 3) {
		return -2;
	}

	string lba = param[1];
	string data = param[2];

	if (lba.length() > 2 || isNumber(lba) == false) {
		return -2;
	}

	if (data.length() != 10) {
		return -2;
	}

	if (data.substr(0, 2).compare("0x") != 0) {
		return -2;
	}

	for (char& c : data.substr(2, string::npos)) {
		if (!(c >= '0' && c <= '9') && !(c >= 'A' && c <= 'F')) {
			return -2;
		}
	}

	string command = "ssd.exe W ";
	command += lba;
	command += " ";
	command += data;

	int ret = system(command.c_str());
	if (ret != 0) {
		return ret;
	}

	if (print2Console) {
		cout << "[Write] Done" << endl;
	}

	return ret;
}
