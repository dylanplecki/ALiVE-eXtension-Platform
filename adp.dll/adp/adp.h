#pragma once

#define WIN32_LEAN_AND_MEAN

// Program Headers
#include "logger.h"
#include "session.h"

// STD Headers
#include <string>

// Windows Headers
#include <Windows.h>

#define ADP_LOG_FILE "log\adp_%Y-%m-%d_%H-%M-%S.log"
#define ADP_LOG_LEVEL adp::logger::info
#define ADP_LOG_FORMAT "[%TimeStamp%] <%Severity%> : %Message%"

/*
	Export function prototype:
		void __stdcall MyExportedFunction(ADP::Session* session, size_t handler_id, const char* input_data);
*/
typedef void(__stdcall *f_export)(adp::session*, size_t, const char*);


namespace adp
{
	std::string current_lib_path;

	std::string lib_path();
	void lib_load();
	void lib_unload();
}
