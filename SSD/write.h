#pragma once

#include "nand.h"

class WriteSSD : public NANDFileHandler {
public:
    bool execute(std::map<int, std::string>& nand, int lba, const std::string& data);

private:
};
