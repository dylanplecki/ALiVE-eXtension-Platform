// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifdef USE_PCH

// OS-Dependent Headers
#if defined(_WIN32) || defined(_WIN64)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

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

// Program Headers
#include <axp/linkages.h>

#endif // USE_PCH