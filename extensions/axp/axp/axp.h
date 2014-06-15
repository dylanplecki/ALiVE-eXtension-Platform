#pragma once

// STD Headers
#include <memory>
#include <string>

#define E_LIB_PATH_NOT_FOUND 0

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
