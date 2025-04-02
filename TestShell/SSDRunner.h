#pragma once
#include <vector>
#include <string>

using namespace std;

class SSDRunner
{
public:
	virtual int execute(vector<string>& param) = 0;

	bool isNumber(string& lba) {
		for (char& c : lba) {
			if (isdigit(c) == false) {
				return false;
			}
		}
		return true;
	}
};