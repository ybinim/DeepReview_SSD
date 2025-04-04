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

	if (isInvalidCommand(commandStr)) {
		return false;
	}

	if ((commandStr != "F") && isInvalidParam(lbaStr, 0, 99)) {
		ofstream outputFile(outputfilePath, ios::trunc);
		outputFile << "ERROR";
		return false;
	}

	if ((commandStr == "E") && isInvalidParam(dataStr, 0, 10)) {
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
		ret = updateCommandBuffer(commandStr, lba, dataStr);
	}

	return ret;
}

bool SSD::isInvalidCommand(string& command) {
	return !((command == "W") || (command == "R") || (command == "E") || (command == "F"));

}

bool SSD::isInvalidParam(string& paramStr, int min, int max) {
	int param = 0;
	bool ret = false;

	try {
		param = stoi(paramStr);

		if (param < min || param > max) {
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