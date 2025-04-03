#pragma once
#include <string>
#include <vector>

#include "SSDExecutor.h"

using namespace std;

class SSDEraser : public SSDExecutor
{
public:
	int execute(vector<string>& param) override;

private:
	int erase(int lba, int size);
};