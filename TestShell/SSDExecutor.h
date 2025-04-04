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

	bool isNumber(string& lba) {
		for (char& c : lba) {
			if (isdigit(c) == false) {
				return false;
			}
		}
		return true;
	}
};