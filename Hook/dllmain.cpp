// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
// Hook.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <SDKDDKVer.h>
#pragma data_seg("mydata")
HHOOK g_hHook = NULL;
HMODULE g_hDllModule;
#pragma data_seg()
//#pragma comment(linker, "/SECTION:mydata,RWS")
LRESULT WINAPI GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code >= 0) {
        MSG* pMsg = reinterpret_cast<MSG*>(lParam);
        KBDLLHOOKSTRUCT* pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        // 判断是否是按键按下事件
        if (wParam == WM_KEYDOWN) {
            std::cout << "按下的键的虚拟键码: " << pKeyboardStruct->vkCode << std::endl;
            // 输出按下的键的虚拟键码
        }
        // 输出消息信息
    }
    //PostMessage(GetConsoleWindow(), WM_USER + 1, 0, 0);
    return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}
BOOL SetGlobalHook()
{
    g_hHook = ::SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)GetMsgProc, g_hDllModule, 0);
    printf("GetMsgProc\n");
    if (NULL == g_hHook) {
        return FALSE;
    }
    return TRUE;
}

BOOL UnsetGlobalHook()
{
    if (g_hHook) {
        ::UnhookWindowsHookEx(g_hHook);
    }
    return TRUE;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        g_hDllModule = hModule;
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}