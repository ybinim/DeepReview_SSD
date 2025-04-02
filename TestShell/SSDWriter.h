#pragma once
#include <string>
#include <vector>

#include "SSDRunner.h"

using namespace std;

class SSDWriter : public SSDRunner
{
public:
	int execute(vector<string>& param) override;
};