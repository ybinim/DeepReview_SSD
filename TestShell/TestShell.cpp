#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

class TestShell
{
public:
	int run(string command) {
        vector<string> param = parseCommand(command, ' ');

        if (param[0].compare("exit") == 0) {
            return 1;
        }
        else if (param[0].compare("read") == 0) {

        }
        else if (param[0].compare("write") == 0) {

        }
        else if (param[0].compare("fullread") == 0) {

        }
        else if (param[0].compare("fullwrite") == 0) {

        }
        else if (param[0].compare("help") == 0) {

        }
        else {
            cout << "INVALID COMMAND" << endl;
            return -1;
        }

        return 0;
	}

private:
    vector<string> parseCommand(string& command, char delimiter) {
        string subParam = "";
        vector<string> result = {};
        istringstream commandStream(command);

        while (getline(commandStream, subParam, delimiter)) {
            result.push_back(subParam);
        }

        return result;
    }
};