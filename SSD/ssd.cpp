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
		LOG_PRINT("Fail - Invalid Command ("+ commandStr +")");
		return false;
	}

	if (IsInvalidLBA(lbaStr))
	{
		std::ofstream outputFile(outputfilePath, std::ios::trunc);
		outputFile << "ERROR";
		return false;
	}

	// update map
	std::map<int, std::string> ssdMap;
	ssdMap.clear();
	ReadFileAndUpdateMap(ssdMap);
	
	// execute
	int lba = stoi(lbaStr);

	if (commandStr == "W")
	{
		ret = myWrite.execute(ssdMap, lba, dataStr);
	}
	else if (commandStr == "R")
	{
		ret = myRead.execute(ssdMap, lba);
	}

	return ret;
}

bool
SSD::IsInvalidCommand(string command)
{
	return !((command == "W") || (command == "R"));

}

bool
SSD::IsInvalidLBA(string lbaStr)
{
	int lba = 0;
	bool ret = false;

	try {
		lba = std::stoi(lbaStr);

		if (lba < 0 || lba >= 100) {
			ret = true;
			LOG_PRINT("Fail - Invalid Command (" + lbaStr + ")");
		}
	}
	catch (const std::invalid_argument& e) {
		ret = true;
	}
	catch (const std::out_of_range& e) {
		ret = true;
	}

	return ret;
}

void 
SSD::ReadFileAndUpdateMap(std::map<int, std::string>& map)
{
	// ssd_nand.txt 파일 읽기
	ifstream file;
	string lineStr, keyStr, valueStr;
	int key = 0xFFFF; // invalid key

	file.open(nandfilePath);
	if (file.is_open())
	{
		// "ssd_nand.txt" 파일이 있는 경우 읽어서 map 구성
		while (getline(file, lineStr)) {
			stringstream mapStream(lineStr);
			mapStream >> keyStr >> valueStr;
			key = stoi(keyStr);
			map.insert({ key , valueStr });
		}
		file.close();
	}
}