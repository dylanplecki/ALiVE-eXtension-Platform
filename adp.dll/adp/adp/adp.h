#pragma once

#define WIN32_LEAN_AND_MEAN

// Program Headers
#include <adp/handler.h>

// STD Headers
#include <string>

// Windows Headers
#include <Windows.h>

#define ADP_LOG_FILE "log\adp_%Y-%m-%d_%H-%M-%S.log"
#define ADP_LOG_LEVEL adp::logger::info
#define ADP_LOG_FORMAT "[%TimeStamp%] <%Severity%> : %Message%"

namespace adp
{
	extern std::string current_lib_path;

	std::string lib_path();
	void lib_load();
	void lib_unload();
}
