// dllmain.cpp : Defines the entry point for the DLL application.

#include "stdafx.h" // Precompiled headers

#include "TranslationUnit.h"


#ifdef _WIN32 // Windows environment

	BOOL APIENTRY DllMain( HMODULE hModule,
						   DWORD  ul_reason_for_call,
						   LPVOID lpReserved
						 )
	{
		switch (ul_reason_for_call)
		{
		case DLL_PROCESS_ATTACH:
			ADP::dyn_lib_load();
			break;
		case DLL_THREAD_ATTACH: // Do nothing
		case DLL_THREAD_DETACH: // Do nothing
		case DLL_PROCESS_DETACH:
			ADP::dyn_lib_unload();
			break;
		}

		return TRUE;
	}

#else // Assume *nix environment

	void __attribute__((constructor)) ADP::dyn_lib_load(void);
	void __attribute__((destructor)) ADP::dyn_lib_unload(void);

#endif // _WIN32


namespace ADP
{
	TranslationUnit* main_translation_unit; // Singleton class

	void dyn_lib_load(void)
	{
		if (main_translation_unit == nullptr) // Create singleton
			main_translation_unit = new TranslationUnit();
	}

	void dyn_lib_unload(void)
	{
		if (main_translation_unit != nullptr) // Delete singleton
			delete main_translation_unit;
	}
}
