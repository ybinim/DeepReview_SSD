#include "gmock/gmock.h"
#include "TestShell.h"
#include "SSDReader.h"
#include "SSDWriter.h"
#include "SSDEraser.h"
#include "SSDFlusher.h"
#include <iostream>
#include <fstream>
#include <string>

#ifdef _DEBUG
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#else
int main(int argc, char* argv[]) {
	std::string command;
	int ret;

	SSDReader reader;
	SSDWriter writer;
	SSDEraser eraser;
	SSDFlusher flusher;
	TestShell* shell = new TestShell(&reader, &writer, &eraser, &flusher);

	if (argc == 1) {
		while (true) {
			std::cout << "Shell> ";
			std::getline(std::cin, command);
			ret = shell->run(command);

			if (ret == 1)
			{
				// exit command
				return 0;
			}
		}
	}
	else if (argc == 2) {
		std::ifstream scriptFile;
		scriptFile.open(argv[1]);
		if (scriptFile.is_open()) {
			while (std::getline(scriptFile, command)) {
				std::cout << command << "   ___   Run...";

				ret = shell->run(command);
				if (ret != 0) {
					break;
				}
			}
			scriptFile.close();
		}
		return 0;
	}

	return -1;
}
#endif