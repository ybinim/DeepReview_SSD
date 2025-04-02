#pragma once
#include <string>
#include <vector>

using namespace std;

class SSDWriter
{
public:
	int write(vector<string>& param);

private:
	bool isNumber(string& lba);
};