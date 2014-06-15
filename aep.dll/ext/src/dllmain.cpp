
// Program Headers
#include <aep/stdafx.h>
#include <aep/aep.h>

#define WIN32_LEAN_AND_MEAN

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
		aep::lib_load();
		break;
	case DLL_THREAD_ATTACH: // Do nothing
	case DLL_THREAD_DETACH: // Do nothing
	case DLL_PROCESS_DETACH:
		aep::lib_unload();
		break;
	}

	return TRUE;
}
