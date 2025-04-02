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
    std::map<int, std::string> myMap;
    bool updateSSDNandFile();
};

#endif