#pragma once
#include <string>
#include <vector>

#include "SSDExecutor.h"

using namespace std;

class SSDReader : public SSDExecutor
{
public:
	int execute(vector<string>& param, bool print2Console = true) override;
	int checkParam(vector<string>& param) override;

private:
	void print(string lba);
	const string outputFilePath = "ssd_output.txt";
};