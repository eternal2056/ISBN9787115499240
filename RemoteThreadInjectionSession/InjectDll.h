#ifndef _INJECT_DLL_H_
#define _INJECT_DLL_H_


#include <Windows.h>


// ʹ�� CreateRemoteThread ʵ��Զ�߳�ע��
BOOL ZwCreateThreadExInjectDll(DWORD dwProcessId, const char* pszDllFileName);


#endif