#include "SSDReader.h"
#include <iostream>
#include <fstream>

using namespace std;

int SSDReader::execute(vector<string>& param, bool print2Console) {
	int ret = 0;

	ret = checkValidity(param);
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

	if (print2Console)
		print2console(lba);

	return ret;
}

int SSDReader::checkValidity(vector<string>& param) {
	if (param.size() != 2) {
		return -2;
	}

	string lba = param[1];

	if (lba.length() > 2 || isNumber(lba) == false) {
		return -2;
	}

	return 0;
}

void SSDReader::print2console(string lba) {
	ifstream outputFile(outputFilePath.c_str());
	string data = "";
	getline(outputFile, data);

	cout << "[Read] LBA " << lba << " : " << data << endl;
}