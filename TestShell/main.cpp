#include <windows.h>   // DLLMain 관련 헤더
#include "gmock/gmock.h"
#include "TestShell.h"
#include "SSDReader.h"
#include "SSDWriter.h"
#include "SSDEraser.h"
#include "SSDFlusher.h"
#include <iostream>
#include <fstream>
#include <string>
#include "TestScript.h"

typedef TestScript* (*CreateTestScript)(); // DLL에서 TestScript 객체 생성 함수
int prePareToUseTestScript(SSDExecutor* reader, SSDExecutor* writer, SSDExecutor* eraser, SSDExecutor* flusher);

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
	prePareToUseTestScript(&reader, &writer, &eraser, &flusher);

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

int prePareToUseTestScript(SSDExecutor* reader, SSDExecutor* writer, SSDExecutor* eraser, SSDExecutor* flusher) {
	// DLL 로드
	HMODULE hDLL = LoadLibrary(L"TestScript.dll");
	if (!hDLL) {
		std::cout << "DLL을 로드할 수 없습니다." << std::endl;
		return -1;
	}

	CreateTestScript createTestScript = (CreateTestScript)GetProcAddress(hDLL, "CreateTestScript");
	if (!createTestScript) {
		std::cout << "CreateTestScript 함수를 찾을 수 없습니다." << std::endl;
		FreeLibrary(hDLL);
		return -1;
	}

	// DLL에서 ShellScript 객체 생성
	TestScript* script = createTestScript();
	if (!script) {
		std::cout << "TestScript 객체 생성 실패" << std::endl;
		FreeLibrary(hDLL);
		return -1;
	}

	// 쉘 스크립트에 명령 추가
	// TestShell 객체를 생성 (std::make_shared 사용)
	std::shared_ptr<TestShell> testShell = std::make_shared<TestShell>(reader, writer, eraser, flusher);
	// TestShell 객체를 TestScript에 추가
	script->addShell(testShell);
	//script->addShell(std::make_shared<TestShell>(reader, writer, eraser, flusher));

	// 실행
	//script->execute();

	// DLL 언로드
	//FreeLibrary(hDLL);

	return 0;
}
#endif