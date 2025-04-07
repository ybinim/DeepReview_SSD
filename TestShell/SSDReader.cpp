#include "SSDReader.h"
#include <iostream>
#include <fstream>

using namespace std;

int SSDReader::execute(vector<string>& param, bool print2Console) {
	int ret = checkParam(param);
	if (ret !=0 ) {
		return ret;
	}

	string lba = param[1];
	string command = "ssd.exe R ";
	command += lba;

	ret = system(command.c_str());
	if (ret != 0) {
		return ret;
	}

	if (print2Console) {
		print(lba);
	}

	return ret;
}

int SSDReader::checkParam(vector<string>& param) {
	if (param.size() != 2) {
		LOG_PRINT("Fail - Invalid parameter size");
		return -2;
	}

	string lba = param[1];

	if (isValidLba(lba) == false) {
		LOG_PRINT("Fail - Invalid LBA format");
		return -2;
	}

	return 0;
}

void SSDReader::print(string lba) {
	ifstream outputFile(outputFilePath.c_str());
	string data = "";
	getline(outputFile, data);
	outputFile.close();

	cout << "[Read] LBA " << lba << " : " << data << endl;
}