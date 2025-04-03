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
void CombineParametersIntoASingleString(std::string& params, char* argv[], int argc);
int main(int argc, char* argv[]) {
    if (argc >= 2 && argc <= 4) {
        std::string params = "";
        CombineParametersIntoASingleString(params, argv, argc);
 
        SSD mySsd;
        mySsd.run(params);
    }

    return 0;
}

void CombineParametersIntoASingleString(std::string& outParams, char* argv[], int argc)
{
    for (int i = 1; i < argc; i++) {
        outParams += argv[i];
        if (i < argc - 1) {
            outParams += " ";
        }
    }
}
#endif
