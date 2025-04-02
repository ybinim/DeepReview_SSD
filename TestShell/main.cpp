#include "gmock/gmock.h"
#include "TestShell.h"
#include "SSDReader.h"
#include "SSDWriter.h"

#ifdef _DEBUG
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#else
int main() {
	std::string command;

	SSDReader reader;
	SSDWriter writer;
	TestShell* shell = new TestShell(&reader, &writer);
	
	while (true) {
		std::cout << "Shell> ";
		std::getline(std::cin, command);
		shell->run(command);
	}
}
#endif