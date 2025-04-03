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
		cout << "invalid" << endl;
		return false;
	}

	if ((commandStr != "F") && isInvalidLBA(lbaStr))
	{
		cout << "invalidinvalid" << endl;
		ofstream outputFile(outputfilePath, ios::trunc);
		outputFile << "ERROR";
		return false;
	}

	// load command buffer
	loadCommandBuffer();

	if (commandStr == "F") {
		ret = flushCommandBuffer();
	}
	else if (commandStr == "R")
	{
		lba = stoi(lbaStr);
		ret = searchInCommandBuffer(lba);
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
		lba = stoi(lbaStr);
		if (commandBuffer.size() == 5) {
			ret = flushCommandBuffer();
			if (ret == false) {
				return ret;
			}
		}
		ret = writeToCommandBuffer(commandStr, lba, dataStr);
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

void SSD::loadCommandBuffer() {
	if (filesystem::exists(bufferDirPath) == false) {
		createEmptyCommandBuffer();
		return;
	}

	for (const auto& file : filesystem::directory_iterator(bufferDirPath)) {
		string fileName = file.path().filename().string();

		if (fileName.substr(1).compare("_empty") == 0) {
			cout << "empty file exists: " << fileName << endl;
			continue;
		}

		stringstream commandStream(fileName);
		string command, lba, param;
		commandStream >> command >> lba >> param;

		cout << "push back to commandBuffer - " << command << lba << param << endl;
		commandBuffer.push_back({ command, stoi(lba), param });
	}
}

void SSD::createEmptyCommandBuffer() {
	filesystem::create_directory(bufferDirPath);
	ofstream empty;
	for (int i = 1; i <= 5; i++) {
		empty.open(bufferDirPath + "/" + to_string(i) + "_empty");
		empty.close();
		cout << "create " << i << " done" << endl;
	}
}

bool SSD::flushCommandBuffer() {
	bool ret = true;

	// update map
	map<int, string> ssdMap;
	ssdMap.clear();
	readFileAndUpdateMap(ssdMap);

	for (const bufferElement& element : commandBuffer) {
		if (element.command == "W") {
			cout << "flush W " << element.lba << element.param << endl;
			ret = myWrite.execute(ssdMap, element.lba, element.param);
		}
		else if (element.command == "E") {
			cout << "flush E " << element.lba << element.param << endl;
			ret = myErase.execute(ssdMap, element.lba, element.param);
		}
		else {
			ret = false;
		}

		if (ret == false) {
			break;
		}
	}

	for (const auto& file : filesystem::directory_iterator(bufferDirPath)) {
		filesystem::remove(file);
	}

	createEmptyCommandBuffer();

	commandBuffer.clear();
	return ret;
}

bool SSD::searchInCommandBuffer(int lba) {
	for (const bufferElement& element : commandBuffer) {
		if (element.command == "W") {
			if (element.lba == lba) {
				cout << "found in W" << endl;
				ofstream outputFile(outputfilePath, ios::trunc);
				outputFile << element.param;
				outputFile.close();
				return true;
			}
		}
		else if (element.command == "E") {
			int size = stoi(element.param);
			if ((lba >= element.lba) && (lba < element.lba + size)) {
				cout << "found in E" << endl;
				ofstream outputFile(outputfilePath, ios::trunc);
				outputFile << "0x00000000";
				outputFile.close();
				return true;
			}
		}
	}

	return false;
}

bool SSD::writeToCommandBuffer(string& command, int lba, string& param)
{
	string targetFileName = bufferDirPath + "/" + to_string(commandBuffer.size() + 1) + "_empty";
	string newFileName = bufferDirPath + "/" + command + " " + to_string(lba) + " " + param;
	filesystem::rename(targetFileName, newFileName);
	return true;
}
