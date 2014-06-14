
// Program Headers
#include <aep/stdafx.h>
#include <aep/library.h>
#include <aep/logger_boost.h>

namespace aep
{
	library::library(const char* lib_path)
	{
		wchar_t win_lib_path[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, lib_path, -1, win_lib_path, MAX_PATH);
		module_ = LoadLibrary(win_lib_path);

		if (!module_)
		{
			AEP_LOG_STREAM_SEV(error) << "Requested library not found at location " << lib_path;
			throw E_LIB_NOT_FOUND;
		}
	}

	library::~library()
	{
		FreeLibrary(module_);
	}

	f_export library::load_function(const char* function_name)
	{
		std::lock_guard<std::mutex> lock(lib_lock_);
		f_export function = functions_[function_name];

		if (!function)
		{
			function = (f_export)GetProcAddress(module_, function_name);

			if (!function)
				AEP_LOG_STREAM_SEV(error) << "Requested function '" << function_name << "' not found";

			functions_[function_name] = function;
		}

		return function;
	}
}