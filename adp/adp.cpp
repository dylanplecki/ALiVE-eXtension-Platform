// adp.cpp : Defines the exported functions for the DLL application.

#include "stdafx.h"

extern "C"
{
	__declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
};

void __stdcall RVExtension(char *output, int outputSize, const char *function)
{
	outputSize -= 1;

	// TODO: Process command

	strncpy(output, function, outputSize);
}
