#pragma once

// STD Headers
#include <memory>
#include <string>

#define AEP_LOG_FILE "log\aep_%Y-%m-%d_%H-%M-%S.log"
#define AEP_LOG_LEVEL aep::logger::info
#define AEP_LOG_FORMAT "[%TimeStamp%] <%Severity%> : %Message%"

namespace aep
{
	class session; // Defined in <src/session.cpp>

	extern std::shared_ptr<session> current_session;
	extern std::string current_lib_path;

	std::string lib_path();
	void lib_load();
	void lib_unload();
	void stop_current_session();
	void start_new_session();
}
