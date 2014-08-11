#if defined(_WIN32) || defined(_WIN64)

// Windows Headers
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


BOOL APIENTRY DllMain( HMODULE hModule,
						DWORD  ul_reason_for_call,
						LPVOID lpReserved
						)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// Do nothing
		break;
	case DLL_THREAD_ATTACH:
		// Do nothing
		break;
	case DLL_THREAD_DETACH:
		// Do nothing
		break;
	case DLL_PROCESS_DETACH:
		// Do nothing
		break;
	}

	return TRUE;
}

#endif