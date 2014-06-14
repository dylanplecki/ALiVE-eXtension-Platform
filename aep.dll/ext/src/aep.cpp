
// Program Headers
#include <aep/stdafx.h>
#include <aep/aep.h>
#include <aep/session.h>
#include <aep/logger_boost.h>
#include <aep/protocol_def.h>

// Boost Headers
#include <boost/utility/string_ref.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


namespace aep
{
	std::shared_ptr<session> current_session;
	std::atomic<std::string> current_lib_path;

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
		logger::initialize((current_lib_path.load() + AEP_LOG_FILE).c_str(), AEP_LOG_FORMAT, AEP_LOG_LEVEL);

		// Log standard information
		AEP_LOG_STREAM_SEV(info) << "ALiVE Data Plugin (ADP) Loaded";
		AEP_LOG_STREAM_SEV(info) << "Working Directory: " << current_lib_path.load();
	}

	void lib_unload()
	{
		// Log standard information
		AEP_LOG_STREAM_SEV(info) << "ALiVE Data Plugin (ADP) Unloaded";
	}

	void stop_current_session()
	{
		if (!current_session)
		{
			current_session.reset();
			AEP_LOG_STREAM_SEV(info) << "Current native session stopped";
		}
	}

	void start_new_session()
	{
		stop_current_session();
		current_session = std::shared_ptr<session>(new session);
		AEP_LOG_STREAM_SEV(info) << "New native session started";
	}
}

extern "C"
{
	__declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
};

// Export RVEngine extension function
void __stdcall RVExtension(char *output, int outputSize, const char *function)
{
	--outputSize;

	if (strlen(function) > 0)
	{
		// Process low-level status flags
		switch (function[0])
		{
		case SF_NEW_SESSION:
			aep::start_new_session();
			break;
		case SF_DEL_SESSION:
			aep::stop_current_session();
			break;
		}
	}

	// Find current session and run
	if (aep::current_session)
		aep::current_session->process_input(function, outputSize, output);
}
