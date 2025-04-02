#pragma once
#include <string>

using namespace std;

class SSDWriter
{
public:
	int write(string lba, string data) {
		if (isNumber(lba) == false) {
			return -2;
		}

		if (data.length() != 10) {
			return -2;
		}

		if (data.substr(0, 2).compare("0x") != 0) {
			return -2;
		}

		string command = "ssd.exe W ";
		command += lba;
		command += " ";
		command += data;

		int ret = system(command.c_str());

		return ret;
	}

private:
	bool isNumber(string& lba)
	{
		for (char& c : lba) {
			if (isdigit(c) == false) {
				return false;
			}
		}
		return true;
	}
};