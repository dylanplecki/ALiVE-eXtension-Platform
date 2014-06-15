
// Program Headers
#include <axp/stdafx.h>
#include <axp/axp.h>
#include <axp/session.h>
#include <axp/logger_boost.h>
#include <axp/protocol_def.h>

#define WIN32_LEAN_AND_MEAN

// Windows Headers
#include <Windows.h>

// Boost Headers
#include <boost/utility/string_ref.hpp>

namespace axp
{
	std::shared_ptr<session> current_session;
	std::string current_lib_path;

	std::string lib_path()
	{
		char path_buffer[MAX_PATH];
		HMODULE hm = NULL;

		if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCSTR)&lib_path,
			&hm))
		{
			int ret = GetLastError();
			BOOST_LOG(logger::global_logger) << "GetModuleHandle returned: " << std::to_string(ret);
		}

		GetModuleFileNameA(hm, path_buffer, sizeof(path_buffer));
		boost::string_ref path = path_buffer;

		return path.substr(0, path.find_last_of("\\") + 1).data();
	}

	void lib_load()
	{
		// Initialize library path variable
		current_lib_path = lib_path();

		// Initialize logger
		logger::initialize((current_lib_path + axp_LOG_FILE).c_str(), axp_LOG_FORMAT, axp_LOG_LEVEL);

		// Log standard information
		AXP_LOG_STREAM_SEV(info) << "ALiVE Data Plugin (ADP) Loaded";
		AXP_LOG_STREAM_SEV(info) << "Working Directory: " << current_lib_path;
	}

	void lib_unload()
	{
		// Log standard information
		AXP_LOG_STREAM_SEV(info) << "ALiVE Data Plugin (ADP) Unloaded";
	}

	void stop_current_session()
	{
		if (!current_session)
		{
			current_session.reset();
			AXP_LOG_STREAM_SEV(info) << "Current native session stopped";
		}
	}

	void start_new_session()
	{
		stop_current_session();
		current_session = std::shared_ptr<session>(new session);
		AXP_LOG_STREAM_SEV(info) << "New native session started";
	}
}

extern "C"
{
	__declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
};

// Export RVEngine extension function
void __stdcall RVExtension(char *output, int output_size, const char *function)
{
	if (output_size > 0)
	{
		if (strlen(function) > 0)
		{
			// Process low-level status flags
			switch (function[0])
			{
			case SF_NEW_SESSION:
				axp::start_new_session();
				break;
			case SF_DEL_SESSION:
				axp::stop_current_session();
				break;
			}
		}

		// Find current session and run
		if (axp::current_session)
			axp::current_session->process_input(function, (output_size - 2), output);

		// Protect against buffer overflows
		output[output_size - 1] = '\0';
	}
}