#ifndef WRITESDD_H
#define WRITESDD_H

#include "nand.h"

class WriteSSD : public NANDFileHandler {
public:
    bool execute(std::map<int, std::string>& nand, int lba, const std::string& data);

private:
};

#endif