
// Program Headers
#include <axp/stdafx.h>
#include <axp/axp.h>
#include <axp/package.h>
#include <axp/session.h>
#include <axp/logger_boost.h>
#include <axp/protocol_def.h>

// STD Headers
#include <iostream>
#include <stdio.h>

// Boost Headers
#include <boost/utility/string_ref.hpp>

// OS-Dependent Headers
#if defined(_WIN32) || defined(_WIN64)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#define AXP_LOG_FILE "log\\axp_%Y-%m-%d_%H-%M-%S.log"
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
			std::string path(path_buffer);
			return path.substr(0, path.find_last_of("\\") + 1);
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
		stop_current_session();

		// Log standard information
		AXP_LOG_STREAM_SEV(info) << "ALiVE Data Plugin (ADP) Unloaded";
	}

	char stop_current_session()
	{
		if (current_session)
		{
			current_session->set_active(false);
			current_session.reset();
			AXP_LOG_STREAM_SEV(info) << "Current native session stopped";
			return SF_GOOD;
		}
		return SF_NONE;
	}

	char start_new_session()
	{
		stop_current_session();
		current_session = std::shared_ptr<session>(new session);
		AXP_LOG_STREAM_SEV(info) << "New native session started";
		return SF_GOOD;
	}
}

#if defined(_WIN32) || defined(_WIN64)
	#define EXPORT_CALL_TYPE __stdcall
	extern "C"
	{
		__declspec(dllexport) void EXPORT_CALL_TYPE RVExtension(char *output, int output_size, const char *function);
	};
#else
	#define EXPORT_CALL_TYPE
#endif

// Export RVEngine extension function
void EXPORT_CALL_TYPE RVExtension(char *output, int output_size, const char *function)
{
	if (output_size > 0)
	{
		// Process low-level status flags
		switch (function[0])
		{
		case SF_NEW_SESSION:
			output[0] = axp::start_new_session();
			output[1] = '\0';
			break;
		case SF_DEL_SESSION:
			output[0] = axp::stop_current_session();
			output[1] = '\0';
			break;
		default:
			output[0] = '\0';
			if (output_size > 1)
			{
				// Find current session and run
				if (axp::current_session)
					axp::current_session->process_input(function, (output_size - 1), output);

				// Protect against non-null-termination
				output[output_size - 1] = '\0';
			}
		}
	}
}

// Standalone Operation System
int main(int argc, const char* argv[])
{
	using namespace std;
	const size_t input_buffer_size = 10240;
	const size_t output_buffer_size = 10240;

	// Initialize library
	axp::lib_load();
	axp::start_new_session();

	while (true) {
		unsigned int status_flag_int;

		do {
			cout << "Please enter a status flag [0,255]: ";
			scanf("%u", &status_flag_int);
			cout << '\n';
		} while (status_flag_int <= 0 && status_flag_int > 255);

		char input_buffer[input_buffer_size];
		char output_buffer[output_buffer_size];

		input_buffer[0] = static_cast<char>(status_flag_int);

		cout << "Please enter a message to send (a tilde may be as a unit separator):" << "\n\n";
		cin >> &(input_buffer[1]);

		for (char &input_char : input_buffer)
		{
			if (input_char == '~')
				input_char = PORTION_DELIMITER;
		}

		cout << "\n\n" << "Processing data..." << "\n\n";

		RVExtension(output_buffer, output_buffer_size, input_buffer);

		cout << "Output:\n\n";
		cout << "    Length: " << strlen(output_buffer) << "\n\n";
		cout << "    Status: " << static_cast<unsigned>(output_buffer[0]) << "\n\n";
		cout << "\n\n" << output_buffer << "\n\n";
		system("pause");
		cout << "\n\n";
	}

	// Close library
	axp::lib_unload();
}
