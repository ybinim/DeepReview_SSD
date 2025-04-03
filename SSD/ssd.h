#pragma once
#include <string>

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
	bool isInvalidLBA(std::string& command);
	void readFileAndUpdateMap(std::map<int, std::string>& map);
	void loadCommandBuffer(std::vector<bufferElement>& commandBuffer);
	void createEmptyCommandBuffer();
	bool flushCommandBuffer(std::vector<bufferElement>& commandBuffer);
	bool searchInCommandBuffer(std::vector<bufferElement>& commandBuffer, int lba);
	bool writeToCommandBuffer(int count, std::string& command, int lba, std::string& param);

	WriteSSD myWrite;
	ReadSSD myRead;
	EraseSSD myErase;

	const std::string outputfilePath = "ssd_output.txt";
	const std::string nandfilePath = "ssd_nand.txt";
	const std::string bufferDirPath = "buffer";
};