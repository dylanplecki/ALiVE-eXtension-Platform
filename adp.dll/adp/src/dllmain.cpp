// dllmain.cpp : Defines the entry point for the DLL application.

#define WIN32_LEAN_AND_MEAN

// Program Headers
#include <adp/stdafx.h>
#include <adp/adp.h>

// Windows Headers
#include <windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
						DWORD  ul_reason_for_call,
						LPVOID lpReserved
						)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		adp::lib_load();
		break;
	case DLL_THREAD_ATTACH: // Do nothing
	case DLL_THREAD_DETACH: // Do nothing
	case DLL_PROCESS_DETACH:
		adp::lib_unload();
		break;
	}

	return TRUE;
}
