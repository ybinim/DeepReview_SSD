#pragma once
#include "nand.h"

class EraseSSD : public NANDFileHandler {
public:
    bool execute(std::map<int, std::string>& nand, int lba, const std::string& size);

private:
    bool GetSizeAfterCheckInvalidSizeOrNot(const std::string& sizeStr, int* getValidSize);

    const std::string outputfilePath = "ssd_output.txt";
};
