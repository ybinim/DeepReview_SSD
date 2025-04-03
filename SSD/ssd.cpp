#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <filesystem>

#include "ssd.h"

using namespace std;

bool SSD::run(string input) {
	bool ret = true;
	int lba = -1;

	stringstream inputStream(input);
	// 스트림을 통해, 문자열을 공백 분리해 변수에 할당
	string commandStr, lbaStr, dataStr;
	inputStream >> commandStr >> lbaStr >> dataStr;

	if (isInvalidCommand(commandStr))
	{
		return false;
	}

	if ((commandStr != "F") && isInvalidLBA(lbaStr))
	{
		ofstream outputFile(outputfilePath, ios::trunc);
		outputFile << "ERROR";
		return false;
	}
	else {
		lba = stoi(lbaStr);
	}

	// load command buffer
	vector<bufferElement> commandBuffer = {};
	loadCommandBuffer(commandBuffer);

	if (commandStr == "F") {
		ret = flushCommandBuffer(commandBuffer);
	}
	else if (commandStr == "R")
	{
		ret = searchInCommandBuffer(commandBuffer, lba);
		if (ret == true) {
			return ret;
		}

		// update map
		map<int, string> ssdMap;
		ssdMap.clear();
		readFileAndUpdateMap(ssdMap);

		// execute
		ret = myRead.execute(ssdMap, lba);
	}
	else if ((commandStr == "W") || (commandStr == "E"))
	{
		if (commandBuffer.size() == 5) {
			ret = flushCommandBuffer(commandBuffer);
			if (ret == false) {
				return ret;
			}
		}
		ret = writeToCommandBuffer(commandBuffer.size() + 1, commandStr, lba, dataStr);
	}

	return ret;
}

bool SSD::isInvalidCommand(string& command) {
	return !((command == "W") || (command == "R") || (command == "E") || (command == "F"));

}

bool SSD::isInvalidLBA(string& lbaStr) {
	int lba = 0;
	bool ret = false;

	try {
		lba = stoi(lbaStr);

		if (lba < 0 || lba >= 100) {
			ret = true;
		}
	}
	catch (const invalid_argument& e) {
		ret = true;
	}
	catch (const out_of_range& e) {
		ret = true;
	}

	return ret;
}

void SSD::readFileAndUpdateMap(map<int, string>& map) {
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

void SSD::loadCommandBuffer(vector<bufferElement>& commandBuffer) {
	if (filesystem::exists(bufferDirPath) == false) {
		createEmptyCommandBuffer();
		return;
	}

	for (const auto& file : filesystem::directory_iterator(bufferDirPath)) {
		string fileName = file.path().filename().string();

		if (fileName.substr(1).compare("_empty") == 0) {
			break;
		}

		stringstream commandStream(fileName);
		string command, lba, param;
		commandStream >> command >> lba >> param;
		commandBuffer.push_back({ command, stoi(lba), param });
	}
}

void SSD::createEmptyCommandBuffer() {
	filesystem::create_directory(bufferDirPath);
	ofstream empty;
	for (int i = 1; i <= 5; i++) {
		empty.open(bufferDirPath + "/" + to_string(i) + "_empty");
		empty.close();
	}
}

bool SSD::flushCommandBuffer(vector<bufferElement>& commandBuffer) {
	bool ret = true;

	// update map
	map<int, string> ssdMap;
	ssdMap.clear();
	readFileAndUpdateMap(ssdMap);

	for (const bufferElement& element : commandBuffer) {
		if (element.command == "W") {
			ret = myWrite.execute(ssdMap, element.lba, element.param);
		}
		else if (element.command == "E") {
			ret = myErase.execute(ssdMap, element.lba, element.param);
		}
		else {
			ret = false;
		}

		if (ret == false) {
			break;
		}
	}

	commandBuffer.clear();
	return ret;
}

bool SSD::searchInCommandBuffer(vector<bufferElement>& commandBuffer, int lba) {
	for (const bufferElement& element : commandBuffer) {
		if (element.command == "W") {
			if (element.lba == lba) {
				ofstream outputFile(outputfilePath, ios::trunc);
				outputFile << element.param;
				outputFile.close();
				return true;
			}
		}
		else if (element.command == "E") {
			int size = stoi(element.param);
			if ((lba >= element.lba) && (lba < element.lba + size)) {
				ofstream outputFile(outputfilePath, ios::trunc);
				outputFile << "0x00000000";
				outputFile.close();
				return true;
			}
		}
	}

	return false;
}

bool SSD::writeToCommandBuffer(int count, string& command, int lba, string& param)
{
	string targetEmptyFile = bufferDirPath + "/" + to_string(count) + "_empty";
	string newFileName = bufferDirPath + "/" + command + " " + to_string(lba) + " " + param;
	filesystem::rename(targetEmptyFile, newFileName);
	return true;
}
