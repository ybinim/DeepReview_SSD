#pragma once
#include <string>
#include <vector>

#include "SSDExecutor.h"

using namespace std;

class SSDReader : public SSDExecutor
{
public:
	int execute(vector<string>& param) override;

private:
	const string outputFilePath = "ssd_output.txt";
};