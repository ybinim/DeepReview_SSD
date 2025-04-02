#pragma once
#include <string>

using namespace std;

class SSDWriter
{
public:
	int write(string lba, string data);

private:
	bool isNumber(string& lba);
};