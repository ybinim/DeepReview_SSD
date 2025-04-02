#include "gmock/gmock.h"
#include <iostream>
#include <string>
#include <exception> 
#include "ssd.h"

#ifdef _DEBUG
int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
#else
bool isInvalidCommand(std::string command);

int main(int argc, char* argv[]) {
    int addr = -1;
    if (argc == 3 || argc == 4) {
        std::string params = "";
        params.append(argv[1]);
        params += " ";
        params.append(argv[2]);

        if (argc == 4) {
            params += " ";
            params.append(argv[3]);
        }

        SSD mySsd;
        bool ret = mySsd.run(params);
    }

    return 0;
}

bool isInvalidCommand(std::string command)
{
    return !((command == "W") || (command == "R") || (command == "w") || (command == "r"));
}

#endif
