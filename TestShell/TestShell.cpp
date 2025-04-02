#include "TestShell.h"
#include <iomanip>

using namespace std;

int TestShell::run(string command) {
    vector<string> param = parseCommand(command, ' ');
    int result = 0;

    if (param[0].compare("exit") == 0) {
        return 1;
    }
    else if (param[0].compare("read") == 0) {
        result = reader->execute(param);
    }
    else if (param[0].compare("write") == 0) {
        result = writer->execute(param);
    }
    else if (param[0].compare("fullread") == 0) {

    }
    else if (param[0].compare("fullwrite") == 0) {

    }
    else if (param[0].compare("help") == 0) {
        printHelp();
    }
    else {
        cout << "INVALID COMMAND" << endl;
        return -1;
    }

    return result;
}

vector<string> TestShell::parseCommand(string& command, char delimiter) {
    string subParam = "";
    vector<string> result = {};
    istringstream commandStream(command);

    while (getline(commandStream, subParam, delimiter)) {
        result.push_back(subParam);
    }

    return result;
}

void TestShell::printHelp() {
    // Title Output
    std::cout << "=====================================\n";
    std::cout << "              Program Help\n";
    std::cout << "=====================================\n\n";

    // Author Information
    std::cout << "Author  : DeepReviewer / ���̳�, �̿���, �ӿ��, �念��\n\n";

    // Usage
    std::cout << "Usage   : Command [arguments] [arguments]\n\n";

    // Commands
    std::cout << "Commands :\n";
    std::cout << "  write [LBA] [DATA]\n";
    std::cout << "  read [LBA]\n";
    std::cout << "  fullwrite [Value]\n";
    std::cout << "  fullread\n";
    std::cout << "  exit\n";

    // Examples
    std::cout << "\nExamples:\n";
    std::cout << std::setw(30) << std::left << "  write 3 0xAAAABBBB"
        << "  : 3�� LBA�� 0xAAAABBBB�� ���\n";
    std::cout << std::setw(30) << std::left << "  read 3"
        << "  : 3�� LBA�� �д´�\n";
    std::cout << std::setw(30) << std::left << "  fullwrite 0xABCDFFFF"
        << "  : ��ü LBA�� 0xABCDFFFF�� ���\n";
    std::cout << std::setw(30) << std::left << "  fullread"
        << "  : ��ü LBA�� ���� ȭ�鿡 ���\n";
    std::cout << std::setw(30) << std::left << "  exit"
        << "  : ���α׷� ����\n";
    // Notes
    std::cout << "\nNote:\n";
    std::cout << "  [LBA]   : 0~99, 10������ �Է¹���\n";
    std::cout << "  [Value] : 0x00000000~0xFFFFFFFF, 0x�� ������ 10 ���ڷ� ǥ��\n";

    // End of Help
    std::cout << "\n=====================================\n";
    std::cout << "              End of Help\n";
    std::cout << "=====================================\n";
}