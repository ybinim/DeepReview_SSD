#include <string>
#include <map>
#include <fstream>

class WriteSSD {
public:
	bool update(std::map<int, std::string>& nand) {
		std::ofstream outFile("ssd_nand.txt");

		outFile.is_open();
		outFile << 2 << " " << "0xCCCCCCCC" << std::endl;
		outFile.close();
	
		return true;
	}

	bool execute(std::map<int, std::string>& nand, int lba, const std::string& data) {
		std::map<int, std::string> myMap = nand;
		myMap.emplace(lba, data);
		return true;
	}
};
