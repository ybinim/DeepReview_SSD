#ifndef ERASESDD_H
#define ERASESDD_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>

class EraseSSD {
public:
    bool execute(std::map<int, std::string>& nand, int lba, const std::string& size);

private:
    bool updateSSDNandFile(std::map<int, std::string>& nand);
    bool GetSizeAfterCheckInvalidSizeOrNot(const std::string& sizeStr, int* getValidSize);

    const std::string outputfilePath = "ssd_output.txt";
    const std::string nandfilePath = "ssd_nand.txt";
    int eraseSize = 0;
};

#endif