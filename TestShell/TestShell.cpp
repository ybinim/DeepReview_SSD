#include "TestShell.h"

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
        if (param.size() != 2) {
            return -2;
        }
        result = runFullWrite(param);
    }
    else if (param[0].compare("help") == 0) {

    }
    else {
        cout << "INVALID COMMAND" << endl;
        return -1;
    }

    return result;
}

int TestShell::runFullWrite(std::vector<std::string>& param)
{
    int result = 0;
    vector<string> fullWriteParam = {};

    string data = param[1];
    for (int i = 0; i < 100; i++) {
        fullWriteParam.push_back("write");
        fullWriteParam.push_back(to_string(i));
        fullWriteParam.push_back(data);

        result = writer->execute(fullWriteParam);
        if (result != 0) {
            break;
        }

        fullWriteParam.clear();
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
