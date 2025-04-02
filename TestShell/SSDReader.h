#pragma once
#include <string>
#include <vector>

#include "SSDRunner.h"

using namespace std;

class SSDReader : public SSDRunner
{
public:
	int execute(vector<string>& param) override;

private:
	const string outputFilePath = "ssd_output.txt";
};