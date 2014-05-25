// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _WIN32 // Windows environment

	// Windows Defines
	#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

	// Windows Headers
	#include <windows.h>

#endif // _WIN32

// Open Standard Headers
#include <stdint.h>

// Standard Library Headers
#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>

// Boost Headers
#include <boost/algorithm/string.hpp>
#include <boost/utility/string_ref.hpp>
