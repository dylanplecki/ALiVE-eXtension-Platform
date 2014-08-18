// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// OS-Dependent Headers
#if defined(_WIN32) || defined(_WIN64)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

// STD Headers
#include <string>

// API Headers
#include <axp\handler.h>
