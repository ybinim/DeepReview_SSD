#ifndef WRITESDD_H
#define WRITESDD_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>

#define PATH_SSD_NAND_FILE  "ssd_nand.txt"

class WriteSSD {
public:
    bool execute(std::map<int, std::string>& nand, int lba, const std::string& data);

private:
    bool updateSSDNandFile(std::map<int, std::string>& nand);
};

#endif