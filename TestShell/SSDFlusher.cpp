#include "SSDFlusher.h"
#include <iostream>

int SSDFlusher::execute(vector<string>& param, bool print2Console) {
	if (param.size() != 1) {
		return -2;
	}

	string command = "ssd.exe F";

	int ret = system(command.c_str());
	if (ret != 0) {
		return ret;
	}

	if (print2Console)
		cout << "[Flush] Done" << endl;

	return ret;
}