#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "ssd.h"

using namespace std;

bool 
SSD::run(string input)
{
	bool ret = true;

	stringstream inputStream(input);
	// 스트림을 통해, 문자열을 공백 분리해 변수에 할당
	string commandStr, lbaStr, dataStr;
	inputStream >> commandStr >> lbaStr >> dataStr;

	if (IsInvalidCommand(commandStr))
	{
		return false;
	}

	int lba = stoi(lbaStr);

	std::map<int, std::string> ssdMap;
	ssdMap.clear();
	ssdMap.insert({ lba , dataStr });

	// ssd_nand.txt 파일 읽기
	ifstream file;
	string filename = "ssd_nand.txt";
	string lineStr, keyStr, valueStr;
	int key = 0xFFFF; // invalid key
	
	file.open(filename);
	if (file.is_open())
	{
		// "ssd_nand.txt" 파일이 있는 경우 읽어서 map 구성
		while (getline(file, lineStr)) {
			stringstream mapStream(lineStr);
			mapStream >> keyStr >> valueStr;
			key = stoi(keyStr);
			ssdMap.insert({ key , valueStr });
		}
		file.close();
	}
	else
	{
		// "ssd_nand.txt" 파일이 없는 경우 file 생성
		ofstream outFile(filename);

		if (!outFile.is_open()) {
			return false;
		}

		outFile.close();
	}

	// execute
	if (commandStr == "W")
	{
		ret = myWrite.execute(ssdMap, lba, dataStr);
	}
	else if (commandStr == "R")
	{

	}

	return ret;
}

bool
SSD::IsInvalidCommand(string command)
{
	return !((command == "W") || (command == "R"));

}