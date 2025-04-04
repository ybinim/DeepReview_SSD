#pragma once
#include <vector>
#include <string>

#include "Logger.h"

using namespace std;

class SSDExecutor
{
public:
	virtual int execute(vector<string>& param, bool print2Console = true) = 0;
	virtual int checkParam(vector<string>& param) = 0;

	bool isValidLba(string& lba) {
		if (lba.length() > 2 || isNumber(lba) == false) {
			return false;
		}
		return true;
	}

	bool isNumber(string& param) {
		for (char& c : param) {
			if (isdigit(c) == false) {
				return false;
			}
		}
		return true;
	}
};