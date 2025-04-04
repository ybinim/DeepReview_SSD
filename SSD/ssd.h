#pragma once
#include <string>
#include <vector>

#include "write.h"
#include "read.h"
#include "erase.h"

struct bufferElement {
	std::string command;
	int lba;
	std::string param;
};

class SSD {
public:
	bool run(std::string input);

private:
	bool isInvalidCommand(std::string& command);
	bool isInvalidParam(std::string& paramStr, int min, int max);
	void readFileAndUpdateMap(std::map<int, std::string>& map);
	void loadCommandBuffer();
	bool flushCommandBuffer();
	bool searchInCommandBuffer(int lba);
	bool updateCommandBuffer(std::string& command, int lba, std::string& param);
	void clearCommandBufferFiles();
	void createCommandBufferFiles();

	WriteSSD myWrite;
	ReadSSD myRead;
	EraseSSD myErase;
	
	std::vector<bufferElement> commandBuffer = {};

	const std::string outputfilePath = "ssd_output.txt";
	const std::string nandfilePath = "ssd_nand.txt";
	const std::string bufferDirPath = "buffer";
};