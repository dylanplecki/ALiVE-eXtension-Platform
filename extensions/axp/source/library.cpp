
// Program Headers
#include <axp/stdafx.h>
#include <axp/library.h>
#include <axp/logger_boost.h>

#if defined(__linux)
	#include <dlfcn.h>
#endif

namespace axp
{
	library::library(const char* lib_path) : lib_path_(lib_path)
	{
		#if defined(_WIN32) || defined(_WIN64)
			wchar_t win_lib_path[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, lib_path, -1, win_lib_path, MAX_PATH);
			module_ = LoadLibrary(win_lib_path);
		#elif defined(__linux)
			module_ = dlopen(lib_path, RTLD_LAZY);
		#else
			#error Cannot preprocess 'axp::library' object constructor: OS not supported
		#endif

		if (!module_)
			throw E_LIB_NOT_FOUND;
	}

	library::~library()
	{
		#if defined(_WIN32) || defined(_WIN64)
			FreeLibrary(module_);
		#elif defined(__linux)
			dlclose(module_);
		#else
			#error Cannot preprocess 'axp::library' object destructor: OS not supported
		#endif
	}

	f_export library::load_function(const char* function_name)
	{
		//std::unique_lock<std::mutex> lock(lib_lock_);
		f_export function = functions_[function_name];

		if (!function)
		{
			try
			{
				#if defined(_WIN32) || defined(_WIN64)
					function = reinterpret_cast<f_export>(GetProcAddress(module_, function_name));
				#elif defined(__linux)
					function = reinterpret_cast<f_export>(dlsym(module_, function_name));
				#else
					#error Cannot preprocess 'axp::library' object member function 'load_function': OS not supported
				#endif
			}
			catch (std::bad_cast error)
			{
				AXP_LOG_STREAM_SEV(error) << "Function '" << function_name << "' in library '" << lib_path_ << "' failed to cast to the correct function prototype: " << error.what();
			}

			if (!function)
				throw E_FUNC_NOT_FOUND;

			functions_[function_name] = function;
		}

		return function;
	}
}
