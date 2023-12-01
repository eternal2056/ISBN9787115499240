// CreateRemoteThread_Test.cpp : 定义控制台应用程序的入口点。
//
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include "InjectDll.h"
#include "AdjustTokenPrivilegesTest.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// 提升当前进程令牌权限
	EnbalePrivileges(GetCurrentProcess());

	// 远线程注入 DLL
#ifndef _WIN64
	BOOL bRet = CreateRemoteThreadInjectDll(9568, L"C:\\D_Files\\Project_Driver\\ISBN9787115499240\\x64\\Debug\\RemoteThreadInjection_Dll.dll");
#else 
	BOOL bRet = CreateRemoteThreadInjectDll(22800);
#endif

	if (FALSE == bRet)
	{
		printf("Inject Dll Error.\n");
	}
	printf("Inject Dll OK.\n");
	system("pause");
	return 0;
}

