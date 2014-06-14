#pragma once

#define WIN32_LEAN_AND_MEAN

// STD Headers
#include <atomic>
#include <memory>
#include <string>

// Windows Headers
#include <Windows.h>

#define ADP_LOG_FILE "log\adp_%Y-%m-%d_%H-%M-%S.log"
#define ADP_LOG_LEVEL adp::logger::info
#define ADP_LOG_FORMAT "[%TimeStamp%] <%Severity%> : %Message%"

namespace adp
{
	class session; // Defined in <src/session.cpp>

	extern std::shared_ptr<session> current_session;
	extern std::atomic<std::string> current_lib_path;

	std::string lib_path();
	void lib_load();
	void lib_unload();
	void stop_current_session();
	void start_new_session();
}
