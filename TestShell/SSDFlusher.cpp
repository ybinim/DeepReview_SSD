#include "SSDFlusher.h"
#include <iostream>

int SSDFlusher::execute(vector<string>& param, bool print2Console) {
	int ret = checkParam(param);
	if (ret != 0) {
		return ret;
	}

	string command = "ssd.exe F";

	ret = system(command.c_str());
	if (ret != 0) {
		return ret;
	}

	if (print2Console)
		cout << "[Flush] Done" << endl;

	return ret;
}

int SSDFlusher::checkParam(vector<string>& param)
{
	if (param.size() != 1) {
		LOG_PRINT("Fail - Invalid parameter size");
		return -2;
	}
	return 0;
}
