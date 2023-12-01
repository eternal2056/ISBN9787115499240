#include "InjectDll.h"


void ShowError(const wchar_t* pszText)
{
	TCHAR szErr[MAX_PATH] = { 0 };
	wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
	::MessageBox(NULL, szErr, "ERROR", MB_OK);
}


// ʹ�� ZwCreateThreadEx ʵ��Զ�߳�ע��
BOOL ZwCreateThreadExInjectDll(DWORD dwProcessId, const char* pszDllFileName)
{
	HANDLE hProcess = NULL;
	SIZE_T dwSize = 0;
	LPVOID pDllAddr = NULL;
	FARPROC pFuncProcAddr = NULL;
	HANDLE hRemoteThread = NULL;
	DWORD dwStatus = 0;

	// ��ע����̣���ȡ���̾��
	hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		ShowError(L"OpenProcess");
		return FALSE;
	}
	// ��ע������������ڴ�
	dwSize = 1 + ::lstrlen(pszDllFileName);
	pDllAddr = ::VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (NULL == pDllAddr)
	{
		ShowError(L"VirtualAllocEx");
		return FALSE;
	}
	// ��������ڴ���д������
	if (FALSE == ::WriteProcessMemory(hProcess, pDllAddr, pszDllFileName, dwSize, NULL))
	{
		ShowError(L"WriteProcessMemory");
		return FALSE;
	}
	// ���� ntdll.dll
	HMODULE hNtdllDll = ::LoadLibrary("ntdll.dll");
	if (NULL == hNtdllDll)
	{
		ShowError(L"LoadLirbary");
		return FALSE;
	}
	// ��ȡLoadLibraryA������ַ
	pFuncProcAddr = ::GetProcAddress(::GetModuleHandle("Kernel32.dll"), "LoadLibraryA");
	if (NULL == pFuncProcAddr)
	{
		ShowError(L"GetProcAddress_LoadLibraryA");
		return FALSE;
	}
	// ��ȡZwCreateThread������ַ
#ifdef _WIN64
	typedef DWORD(WINAPI* typedef_ZwCreateThreadEx)(
		PHANDLE ThreadHandle,
		ACCESS_MASK DesiredAccess,
		LPVOID ObjectAttributes,
		HANDLE ProcessHandle,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		ULONG CreateThreadFlags,
		SIZE_T ZeroBits,
		SIZE_T StackSize,
		SIZE_T MaximumStackSize,
		LPVOID pUnkown);
#else
	typedef DWORD(WINAPI* typedef_ZwCreateThreadEx)(
		PHANDLE ThreadHandle,
		ACCESS_MASK DesiredAccess,
		LPVOID ObjectAttributes,
		HANDLE ProcessHandle,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		BOOL CreateSuspended,
		DWORD dwStackSize,
		DWORD dw1,
		DWORD dw2,
		LPVOID pUnkown);
#endif
	typedef_ZwCreateThreadEx ZwCreateThreadEx = (typedef_ZwCreateThreadEx)::GetProcAddress(hNtdllDll, "ZwCreateThreadEx");
	if (NULL == ZwCreateThreadEx)
	{
		ShowError(L"GetProcAddress_ZwCreateThread");
		return FALSE;
	}
	// ʹ�� ZwCreateThreadEx ����Զ�߳�, ʵ�� DLL ע��
	dwStatus = ZwCreateThreadEx(&hRemoteThread, PROCESS_ALL_ACCESS, NULL, hProcess, (LPTHREAD_START_ROUTINE)pFuncProcAddr, pDllAddr, 0, 0, 0, 0, NULL);
	if (NULL == hRemoteThread)
	{
		ShowError(L"ZwCreateThreadEx");
		return FALSE;
	}
	// �رվ��
	::CloseHandle(hProcess);
	::FreeLibrary(hNtdllDll);

	return TRUE;
}