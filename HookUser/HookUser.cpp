#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>

// ���߳���Ϣ������
LRESULT CALLBACK MainThreadCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_USER + 1) {
		printf("wefwef\n");
			// ��ȡ�����¼���Ϣ
			KBDLLHOOKSTRUCT* pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

			std::cout << "���µļ����������: " << pKeyboardStruct->vkCode << std::endl;
			// �ж��Ƿ��ǰ��������¼�
			if (wParam == WM_KEYDOWN) {
				// ������µļ����������
			}
	}

	// ����������Ϣ��Ĭ�ϵĴ��ڹ���
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
		// �������ش���
		HWND hwnd = CreateWindow(L"MyWindowClass", L"MyHiddenWindow", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
		if (hwnd == NULL) {
			std::cerr << "�޷��������ش���" << std::endl;
			return 1;
		}
		// ��Ϣѭ�������ֳ�������
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

