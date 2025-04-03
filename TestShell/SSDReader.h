#pragma once
#include <string>
#include <vector>

#include "SSDExecutor.h"

using namespace std;

class SSDReader : public SSDExecutor
{
public:
	int execute(vector<string>& param, bool print2Console = true) override;
	int checkValidity(vector<string>& param);
	void print2console(string lba);

private:
	const string outputFilePath = "ssd_output.txt";
};