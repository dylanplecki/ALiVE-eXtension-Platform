
// Program Headers
#include "stdafx.h"
#include "adp.h"
#include "logger_boost.h"

// Boost Headers
#include <boost/utility/string_ref.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


namespace adp
{
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
			BOOST_LOG(adp::logger::global_logger) << "";
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
		adp::logger::initialize((current_lib_path + ADP_LOG_FILE).c_str(), ADP_LOG_FORMAT, ADP_LOG_LEVEL);

		// Log standard information
		ADP_LOG_STREAM_SEV(info) << "ALiVE Data Plugin (ADP) Loaded";
		ADP_LOG_STREAM_SEV(info) << "Working Directory: " << current_lib_path;

		// TODO - Initialization
	}

	void lib_unload()
	{
		// TODO - Cleanup

		// Log standard information
		ADP_LOG_STREAM_SEV(info) << "ALiVE Data Plugin (ADP) Unloaded";
	}
}

extern "C"
{
	__declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
};

// Export RVEngine extension function
void __stdcall RVExtension(char *output, int outputSize, const char *function)
{
	// TODO
}
