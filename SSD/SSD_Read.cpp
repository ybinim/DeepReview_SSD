#include <iostream>
#include <string>
#include <map>
#include <fstream>

class ReadSSD {
public:
	void execute(const std::map<int, std::string>& nand, int addr) {
		std::ofstream outputFile(filePath, std::ios::trunc);
		if (!outputFile.is_open()) {
			std::cout << "Failed to open file : " << filePath << std::endl;
			return;
		}

		if (addr < 0 || addr >= 100) {
			outputFile << "ERROR";
			outputFile.close();
			return;
		}

		auto it = nand.find(addr);
		if (it == nand.end()) {
			outputFile << VALUE_ZERO;
			outputFile.close();
			return;
		}

		outputFile << it->second;
		outputFile.close();
		return;
	}

private:
	const std::string filePath = "ssd_output.txt";
	const std::string VALUE_ZERO = "0x00000000";
};