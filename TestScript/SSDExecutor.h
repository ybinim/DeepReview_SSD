#pragma once
#include <vector>
#include <string>

using namespace std;

class SSDExecutor
{
public:
	virtual int execute(vector<string>& param, bool print2Console = true) = 0;
};