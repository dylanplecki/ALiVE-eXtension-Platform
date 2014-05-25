// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Generic Defines
#define LOG_FILE "log\adp_%Y-%m-%d_%H-%M-%S.log"
#define LOG_LEVEL boost::log::trivial::info
#define LOG_FORMAT "[%TimeStamp%] <%Severity%> : %Message%"

#ifdef _WIN32 // Windows environment

	// Windows Defines
	#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

	// Windows Headers
	#include <windows.h>

#endif // _WIN32

// Open Standard Headers
#include <stdint.h>

// Standard Library Headers
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

// Boost Headers
#include <boost/algorithm/string.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/utility/string_ref.hpp>
