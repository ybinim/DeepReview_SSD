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
    std::cout << "Argument count : " << argc << std::endl;

    int addr = -1;
    if (argc == 3 || argc == 4) {
        if (isInvalidCommand(argv[1])) {
            return 0;
        }

        try {
            addr = std::stoi(argv[2]);
            std::cout << "Converted number: " << addr << std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Invalid argument: " << e.what() << std::endl;
            return 0;
        }
        catch (const std::out_of_range& e) {
            std::cout << "Out of range: " << e.what() << std::endl;
            return 0;
        }

        if (addr < 0 || addr >= 100) {
            std::cout << "addr : " << addr << std::endl;
            return 0;
        }

        std::string params = "";
        params.append(argv[1]);
        params += " ";
        params.append(argv[2]);

        if (argc == 4) {
            try {
                // 16������ ��ȯ (stoul�� unsigned long���� ��ȯ�ǹǷ�, ������ ���� Ȯ�� �ʿ�)
                unsigned int addr = std::stoul(argv[3], nullptr, 16);  // 16�� �� ��° ���ڷ� �����Ͽ� 16������ ��ȯ
                printf("0x%x\n", addr);
                params += " ";
                params.append(argv[3]);
                std::cout << "argv[3] : " << argv[3] << std::endl;
            }
            catch (const std::invalid_argument& e) {
                std::cout << "Invalid argument: " << e.what() << std::endl;
                return 0;
            }
            catch (const std::out_of_range& e) {
                std::cout << "Out of range: " << e.what() << std::endl;
                return 0;
            }
        }

        std::cout << "params : " << params << std::endl;

        SSD mySsd;
        bool ret = mySsd.run(params);
        std::cout << "ret : " << ret << std::endl;
    }

    return 0;
}

bool isInvalidCommand(std::string command)
{
    return !((command == "W") || (command == "R") || (command == "w") || (command == "r"));
}

#endif
