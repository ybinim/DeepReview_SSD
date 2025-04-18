#pragma once
#include <string>
#include <vector>

#include "write.h"
#include "read.h"
#include "erase.h"

using namespace std;

struct bufferElement {
	std::string command;
	int lba;
	std::string param;
};

class SSD {
public:
	bool run(std::string input);

private:
	bool isInvalidLBA(string lbaStr);
	bool isInvalidParam(vector<string>& param);
	void readFileAndUpdateMap(std::map<int, std::string>& map);
	void loadCommandBuffer();
	bool flushCommandBuffer();
	bool searchInCommandBuffer(int lba);
	bool updateCommandBuffer(std::string& command, int lba, std::string& param);
	void clearCommandBufferFiles();
	void createCommandBufferFiles();
	vector<string> parseString(string& str, char delimiter);

	WriteSSD myWrite;
	ReadSSD myRead;
	EraseSSD myErase;
	
	std::vector<bufferElement> commandBuffer = {};

	const std::string outputfilePath = "ssd_output.txt";
	const std::string nandfilePath = "ssd_nand.txt";
	const std::string bufferDirPath = "buffer";
};