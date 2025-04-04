#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>

class NANDFileHandler {
public:
    bool updateSSDNandFile(std::map<int, std::string>& nand) {
        std::ofstream outFile(nandfilePath);

        if (!outFile.is_open()) {
            return false;
        }

        for (const auto& pair : nand) {
            outFile << pair.first << " " << pair.second << std::endl;
        }

        outFile.close();
        return true;
    }

private:
    const std::string nandfilePath = "ssd_nand.txt";
};