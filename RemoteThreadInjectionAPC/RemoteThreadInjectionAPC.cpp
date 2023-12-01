// RemoteThreadInjectionAPC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <SDKDDKVer.h>
#include "ApcInject.h"
int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bRet = FALSE;

	// APC×¢Èë
#ifdef _WIN64
	bRet = ApcInjectDll("explorer.exe", "C:\\D_Files\\Project_Driver\\ISBN9787115499240\\x64\\Debug\\RemoteThreadInjection_Dll.dll");
#else
	bRet = ApcInjectDll("explorer.exe", "C:\\Users\\DemonGan\\Desktop\\APC_Test\\Debug\\TestDll.dll");
#endif
	if (bRet)
	{
		printf("APC Inject OK.\n");
	}
	else
	{
		printf("APC Inject ERROR.\n");
	}

	system("pause");
	return 0;
}