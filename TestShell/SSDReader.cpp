#include "SSDReader.h"
#include <iostream>
#include <fstream>

using namespace std;

int SSDReader::execute(vector<string>& param) {
	if (param.size() != 2) {
		return -2;
	}

	string lba = param[1];

	if (lba.length() > 2 || isNumber(lba) == false) {
		return -2;
	}

	string command = "ssd.exe R ";
	command += lba;

	int ret = system(command.c_str());
	if (ret != 0) {
		return ret;
	}

	ifstream outputFile(outputFilePath.c_str());
	string data = "";
	getline(outputFile, data);

	cout << "[Read] LBA " << lba << " : " << data;

	return ret;
}
