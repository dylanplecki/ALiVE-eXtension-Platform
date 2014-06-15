#pragma once

// STD Headers
#include <memory>
#include <string>

#define axp_LOG_FILE "log\axp_%Y-%m-%d_%H-%M-%S.log"
#define axp_LOG_LEVEL axp::logger::info
#define axp_LOG_FORMAT "[%TimeStamp%] <%Severity%> : %Message%"

namespace axp
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
