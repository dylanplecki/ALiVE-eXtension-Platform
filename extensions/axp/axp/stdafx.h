// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifdef USE_PCH

#ifdef _WIN32 // Windows environment

	// Windows Defines
	#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

	// Windows Headers
	#include <windows.h>

#endif // _WIN32

// Standard Library Headers
#include <algorithm>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// Boost Headers
#include <boost/utility/string_ref.hpp>

#endif // USE_PCH