#pragma once
#include <string>

using namespace std;

class TestShell
{
public:
	int run(string param) {
		if (param.compare("exit") == 0) {
			return 1;
		}

		return 0;
	}
};