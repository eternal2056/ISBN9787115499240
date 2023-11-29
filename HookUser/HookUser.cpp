#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>

// 主线程消息处理函数
LRESULT CALLBACK MainThreadCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_USER + 1) {
		printf("wefwef\n");
			// 获取键盘事件信息
			KBDLLHOOKSTRUCT* pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

			std::cout << "按下的键的虚拟键码: " << pKeyboardStruct->vkCode << std::endl;
			// 判断是否是按键按下事件
			if (wParam == WM_KEYDOWN) {
				// 输出按下的键的虚拟键码
			}
	}

	// 继续传递消息给默认的窗口过程
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int _tmain(int argc, _TCHAR* argv[])
{
	typedef BOOL(*typedef_SetGlobalHook)();
	typedef BOOL(*typedef_UnsetGlobalHook)();
	HMODULE hDll = NULL;
	typedef_SetGlobalHook SetGlobalHook = NULL;
	typedef_UnsetGlobalHook UnsetGlobalHook = NULL;
	BOOL bRet = FALSE;

	do
	{
		hDll = ::LoadLibrary(L"Hook.dll");
		if (NULL == hDll)
		{
			printf("LoadLibrary Error[%d]\n", ::GetLastError());
			break;
		}
		SetGlobalHook = (typedef_SetGlobalHook)::GetProcAddress(hDll, "SetGlobalHook");
		if (NULL == SetGlobalHook)
		{
			printf("GetProcAddress Error[%d]\n", ::GetLastError());
			break;
		}
		bRet = SetGlobalHook();
		if (bRet)
		{
			printf("SetGlobalHook OK.\n");
		}
		else
		{
			printf("SetGlobalHook ERROR.\n");
		}

		WNDCLASS wc = {};
		wc.lpfnWndProc = MainThreadCallback;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = L"MyWindowClass";
		RegisterClass(&wc);
		// 创建隐藏窗口
		HWND hwnd = CreateWindow(L"MyWindowClass", L"MyHiddenWindow", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
		if (hwnd == NULL) {
			std::cerr << "无法创建隐藏窗口" << std::endl;
			return 1;
		}
		// 消息循环，保持程序运行
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		system("pause");

		UnsetGlobalHook = (typedef_UnsetGlobalHook)::GetProcAddress(hDll, "UnsetGlobalHook");
		if (NULL == UnsetGlobalHook)
		{
			printf("GetProcAddress Error[%d]\n", ::GetLastError());
			break;
		}
		UnsetGlobalHook();
		printf("UnsetGlobalHook OK.\n");

	} while (FALSE);

	system("pause");
	return 0;
}

