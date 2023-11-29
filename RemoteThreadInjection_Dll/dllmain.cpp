// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	::MessageBox(NULL, L"This Is From Dll!\nInject Success!", L"OK", MB_OK);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		::MessageBox(NULL, L"This Is From Dll!\nInject Success!", L"OK", MB_OK);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

