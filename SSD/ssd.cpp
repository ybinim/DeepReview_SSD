﻿#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <filesystem>

#include "ssd.h"

using namespace std;

bool SSD::run(string input) {
	bool ret = true;
	int lba = -1;

	vector<string> param = parseString(input, ' ');
	string commandStr = param[0];
	string lbaStr = "";
	string dataStr = "";

	if (isInvalidParam(param)) {
		return false;
	}

	// load command buffer
	loadCommandBuffer();

	if (commandStr == "F") {
		ret = flushCommandBuffer();
	}
	else if (commandStr == "R")
	{
		lbaStr = param[1];
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
		lbaStr = param[1];
		lba = stoi(lbaStr);
		if (commandBuffer.size() == 5) {
			ret = flushCommandBuffer();
			if (ret == false) {
				return ret;
			}
		}

		dataStr = param[2];
		ret = updateCommandBuffer(commandStr, lba, dataStr);
	}

	return ret;
}

bool
SSD::isInvalidLBA(string lbaStr)
{
	int lba = 0;
	bool ret = false;

	try {
		lba = std::stoi(lbaStr);

		if (lba < 0 || lba >= 100) {
			ret = true;
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

bool SSD::isInvalidParam(vector<string>& param) {
	string commandStr = param[0];

	if (commandStr == "F")
	{
		if (param.size() != 1) {
			return true;
		}
	}
	else
	{
		string lbaStr = param[1];
		if (isInvalidLBA(lbaStr))
		{
			ofstream outputFile(outputfilePath, ios::trunc);
			outputFile << "ERROR";
			return true;
		}

		if (commandStr == "R")
		{
			if (param.size() != 2) {
				return true;
			}
		}
		else if (commandStr == "W")
		{
			if (param.size() != 3) {
				return true;
			}

			string dataStr = param[2];
			if (dataStr.length() != 10) {
				return true;
			}

			if (dataStr.substr(0, 2).compare("0x") != 0) {
				return true;
			}

			for (char& c : dataStr.substr(2, string::npos)) {
				if (!(c >= '0' && c <= '9') && !(c >= 'A' && c <= 'F')) {
					return true;
				}
			}
		}
		else if (commandStr == "E")
		{
			if (param.size() != 3) {
				return true;
			}

			string sizeStr = param[2];
			try {
				int size = std::stoi(sizeStr);

				if (size < 0 || size > 10) {
					ofstream outputFile(outputfilePath, ios::trunc);
					outputFile << "ERROR";
					return true;
				}
			}
			catch (const std::invalid_argument& e) {
				return true;
			}
			catch (const std::out_of_range& e) {
				return true;
			}

		}
		else {
			return true; // Invalid Command
		}
	}

	return false;
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
		// create directory and empty files
		filesystem::create_directory(bufferDirPath);
		createCommandBufferFiles();
		return;
	}

	commandBuffer.clear();

	for (const auto& file : filesystem::directory_iterator(bufferDirPath)) {
		string fileName = file.path().filename().string();

		if (fileName.substr(1).compare("_empty") == 0) {
			continue;
		}

		vector<string> temp_param = parseString(fileName, '_');
		string command = temp_param[1];
		string lba = temp_param[2];

		string param = "";
		auto it = temp_param.begin();
		it = it + 3;
		while (it != temp_param.end()) {
			param += *it;
			it++;
		}

		commandBuffer.push_back({ command, stoi(lba), param });
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
	clearCommandBufferFiles();
	createCommandBufferFiles();
	return ret;
}

bool SSD::searchInCommandBuffer(int lba) {
	auto it = commandBuffer.rbegin();

	while (it != commandBuffer.rend()) {
		if (it->command == "W") {
			if (it->lba == lba) {
				ofstream outputFile(outputfilePath, ios::trunc);
				outputFile << it->param;
				outputFile.close();
				return true;
			}
		}
		else if (it->command == "E") {
			int size = stoi(it->param);
			if ((lba >= it->lba) && (lba < it->lba + size)) {
				ofstream outputFile(outputfilePath, ios::trunc);
				outputFile << "0x00000000";
				outputFile.close();
				return true;
			}
		}
		it++;
	}

	return false;
}

bool SSD::updateCommandBuffer(string& command, int lba, string& param)
{
	bool erased;
	bool skip = false;
	auto it = commandBuffer.rbegin();

	while (it != commandBuffer.rend()) {
		erased = false;
		if (command == "W") {
			if ((it->command == "W") && (it->lba == lba)) {
				// write to same LBA -> remove & push back to manage command order
				it = vector<bufferElement>::reverse_iterator(commandBuffer.erase((++it).base()));
				erased = true;
				break;
			}
		}
		else if (command == "E") {
			int size = stoi(param);
			if (it->command == "W") {
				if ((it->lba >= lba) && (it->lba < lba + size)) {
					// erase after write -> remove write
					it = vector<bufferElement>::reverse_iterator(commandBuffer.erase((++it).base()));
					erased = true;
				}
			}
			else if (it->command == "E") {
				int elememtSize = stoi(it->param);

				if ((it->lba >= lba) && (it->lba + elememtSize <= lba + size)) {
					// erase wide range -> remove narrow one 
					it = vector<bufferElement>::reverse_iterator(commandBuffer.erase((++it).base()));
					erased = true;
				}
				else if ((it->lba <= lba) && (it->lba + elememtSize >= lba + size)) {
					// erase narrow range -> no need to update command buffer
					skip = true;
					break;
				}
				else if (it == commandBuffer.rbegin()) // Erase Command 가 연속적으로 2회 들어왔을때
				{
					int commandEnd = lba + size - 1;
					int itEnd = it->lba + elememtSize - 1;
					// merge range
					if (!(itEnd < lba || it->lba > commandEnd))
					{
						int newlba = min(it->lba, lba);
						int newEnd = max(commandEnd, itEnd);
						int newSize = newEnd - newlba + 1;

						if (newSize <= 10)
						{
							// update param
							it->lba = newlba;
							it->param = to_string(newSize);
							skip = true;
						}						
					}
					break;
				}
			}
		}

		if (!erased) {
			it++;
		}
	}

	if (!skip) {
		commandBuffer.push_back({ command, lba, param });
	}

	//for (bufferElement& element : commandBuffer) {
	//	cout << "command buffer after - " << element.command << element.lba << element.param << endl;
	//}

	clearCommandBufferFiles();
	createCommandBufferFiles();
	return true;
}

void SSD::clearCommandBufferFiles() {
	for (const auto& file : filesystem::directory_iterator(bufferDirPath)) {
		filesystem::remove(file);
	}
}

void SSD::createCommandBufferFiles() {
	int count = 1;
	ofstream file;
	for (bufferElement& element : commandBuffer) {
		file.open(bufferDirPath + "/" + to_string(count) + "_" + element.command + "_" + to_string(element.lba) + "_" + element.param);
		file.close();
		count++;
	}
	while (count <= 5) {
		file.open(bufferDirPath + "/" + to_string(count) + "_empty");
		file.close();
		count++;
	}
}

vector<string> SSD::parseString(string& str, char delimiter) {
	string subParam = "";
	vector<string> result = {};
	istringstream commandStream(str);

	while (getline(commandStream, subParam, delimiter)) {
		result.push_back(subParam);
	}

	return result;
}