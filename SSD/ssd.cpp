#include <sstream>
#include "ssd.h"

using namespace std;

bool 
SSD::run(string input)
{
	stringstream inputStream(input);
	// 스트림을 통해, 문자열을 공백 분리해 변수에 할당
	string command, lba, data;
	inputStream >> command >> lba >> data;

	if (IsInvalidCommand(command))
	{
		return false;
	}

	return true;
}

bool
SSD::IsInvalidCommand(string command)
{
	return !((command == "W") || (command == "R"));

}