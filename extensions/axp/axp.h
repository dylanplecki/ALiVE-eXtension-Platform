#pragma once

// Program Headers
#include <axp/shared_ptr.h>

// STD Headers
#include <memory>
#include <string>


// Defines
#define E_LIB_PATH_NOT_FOUND 0

// Macros
#define OUTPUT_STRING(value) \
	{ \
		std::string temp_record; \
		temp_record.push_back('\"'); \
		temp_record.append(value); \
		temp_record.push_back('\"'); \
		strncpy(output_buffer, temp_record.c_str(), output_size); \
	}


namespace axp
{
	class session; // Defined in <src/session.cpp>

	extern axp::shared_ptr<session> current_session;
	extern std::string current_lib_path;

	std::string lib_path();
	void lib_load();
	void lib_unload();
	char stop_current_session();
	char start_new_session();
}
