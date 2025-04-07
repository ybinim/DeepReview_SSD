﻿// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
//#include "pch.h"
#include <windows.h>   // DLLMain 관련 헤더
#include <iostream>    // 표준 출력 등을 위한 헤더
#include <string>      // std::string 사용을 위한 헤더

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

