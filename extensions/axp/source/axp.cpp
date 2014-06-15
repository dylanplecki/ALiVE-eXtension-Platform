
// Program Headers
#include <axp/stdafx.h>
#include <axp/axp.h>
#include <axp/session.h>
#include <axp/logger_boost.h>
#include <axp/protocol_def.h>

// Boost Headers
#include <boost/utility/string_ref.hpp>

// OS-Dependent Headers
#if defined(_WIN32) || defined(_WIN64)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#define AXP_LOG_FILE "log\axp_%Y-%m-%d_%H-%M-%S.log"
	#define LIB_LOAD_ATTR
	#define LIB_UNLOAD_ATTR
#elif defined(__linux)
	#define AXP_LOG_FILE "log/axp_%Y-%m-%d_%H-%M-%S.log"
	#define LIB_LOAD_ATTR __attribute__((constructor))
	#define LIB_UNLOAD_ATTR __attribute__((destructor))
#else
	#error Cannot load log file path: OS not supported
#endif

#define AXP_LOG_LEVEL axp::logger::info
#define AXP_LOG_FORMAT "[%TimeStamp%] <%Severity%> : %Message%"

namespace axp
{
	std::shared_ptr<session> current_session;
	std::string current_lib_path;

	std::string lib_path()
	{
		#if defined(_WIN32) || defined(_WIN64)
			char path_buffer[MAX_PATH];
			HMODULE hm = NULL;

			if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
				GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				(LPCSTR)&lib_path,
				&hm))
			{
				int ret = GetLastError();
				BOOST_LOG(logger::global_logger) << "Cannot get dynamic library file path (via GetModuleHandleExA), returned: " << std::to_string(ret);
				throw E_LIB_PATH_NOT_FOUND;
			}

			GetModuleFileNameA(hm, path_buffer, sizeof(path_buffer));
			boost::string_ref path(path_buffer);
			return path.substr(0, path.find_last_of("\\") + 1).data();
		#elif defined(__linux)
			Dl_info* info_struct(new Dl_info);

			if (!dladdr(&lib_path, info_struct))
			{
				BOOST_LOG(logger::global_logger) << "Cannot get shared library file path (via dladdr)";
				throw E_LIB_PATH_NOT_FOUND;
			}

			boost::string_ref path(info_struct.dli_fname);
			return path.substr(0, path.find_last_of("/") + 1).data();
		#else
			#error Cannot preprocess function 'axp::lib_path': OS not supported
		#endif
	}

	LIB_LOAD_ATTR void lib_load()
	{
		// Initialize library path variable
		current_lib_path = lib_path();

		// Initialize logger
		logger::initialize((current_lib_path + AXP_LOG_FILE).c_str(), AXP_LOG_FORMAT, AXP_LOG_LEVEL);

		// Log standard information
		AXP_LOG_STREAM_SEV(info) << "ALiVE Data Plugin (ADP) Loaded";
		AXP_LOG_STREAM_SEV(info) << "Working Directory: " << current_lib_path;
	}

	LIB_UNLOAD_ATTR void lib_unload()
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

#if defined(_WIN32) || defined(_WIN64)
	#define EXPORT_CALL_TYPE __stdcall
	extern "C"
	{
		__declspec(dllexport) void EXPORT_CALL_TYPE RVExtension(char *output, int outputSize, const char *function);
	};
#else
	#define EXPORT_CALL_TYPE
#endif

// Export RVEngine extension function
void EXPORT_CALL_TYPE RVExtension(char *output, int output_size, const char *function)
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
