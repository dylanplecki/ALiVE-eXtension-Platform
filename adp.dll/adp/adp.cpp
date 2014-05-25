// adp.cpp : Defines the exported functions for the DLL application.

#include "stdafx.h" // Precompiled headers

#include "TranslationUnit.h"


namespace ADP
{
	extern TranslationUnit* main_translation_unit; // Defined in dllmain.cpp
}


#ifdef _WIN32 // Windows environment

	extern "C"
	{
		__declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
	};

	#define EXPORT_FUNCTION_CALL __stdcall

#else // Not a Windows environment

	#define EXPORT_FUNCTION_CALL	// None

#endif // _WIN32


// Export RVEngine extension function
void EXPORT_FUNCTION_CALL RVExtension(char *output, int outputSize, const char *function)
{
	if (ADP::main_translation_unit != nullptr) // Safety check
		ADP::main_translation_unit->Receive(function, (outputSize - 1), output);
}
