#pragma once
#include <string>
#include <vector>

#include "SSDExecutor.h"

using namespace std;

class SSDWriter : public SSDExecutor
{
public:
	int execute(vector<string>& param) override;
};