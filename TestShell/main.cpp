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

typedef TestScript* (*CreateTestScriptFunc)(); // DLL에서 TestScript 객체 생성 함수
int prePareToUseTestScript(TestScriptCallback& cb, TestShell* shell);

#ifdef _DEBUG
int main() {
	::testing::InitGoogleMock();
	RUN_ALL_TESTS();
	std::cout << "엔터를 누르면 종료됩니다...";
	std::cin.get();
	return 0;
}
#else
int main(int argc, char* argv[]) {
	std::string command;
	int ret;

	// 1. SSDExecutor 인스턴스 생성
	static SSDReader reader;
	static SSDWriter writer;
	static SSDEraser eraser;
	static SSDFlusher flusher;

	TestShell* shell = new TestShell(&reader, &writer, &eraser, &flusher);

	// 2. 콜백 구조체 세팅
	TestScriptCallback cb;
	cb.reader = &reader;
	cb.writer = &writer;
	cb.eraser = &eraser;
	cb.flusher = &flusher;

	prePareToUseTestScript(cb, shell);

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

int prePareToUseTestScript(TestScriptCallback& cb, TestShell* shell) {
	// DLL 로드
	HMODULE hDLL = LoadLibrary(L"TestScript.dll");
	if (!hDLL) {
		std::cout << "DLL을 로드할 수 없습니다." << std::endl;
		return -1;
	}
	
	CreateTestScriptFunc createTestScript = (CreateTestScriptFunc)GetProcAddress(hDLL, "CreateTestScript");
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

	// TestScript 객체를 TestShell에 등록
	std::shared_ptr<TestScript> scriptPtr(script);
	
	shell->setTestScript(scriptPtr);  // TestScript 등록

	script->registerCallback(&cb);

	// DLL 언로드
	//FreeLibrary(hDLL);

	return 0;
}

#endif