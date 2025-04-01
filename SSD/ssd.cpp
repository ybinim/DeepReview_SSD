#include "ssd.h"

using namespace std;

bool 
SSD::run(string input)
{
	string command = input.substr(0, 1);

	if (command == "W" || command == "R")
	{
		return true;
	}

	return false;
}