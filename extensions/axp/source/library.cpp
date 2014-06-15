
// Program Headers
#include <axp/stdafx.h>
#include <axp/library.h>

namespace axp
{
	library::library(const char* lib_path) : lib_path_(lib_path)
	{
		wchar_t win_lib_path[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, lib_path, -1, win_lib_path, MAX_PATH);
		module_ = LoadLibrary(win_lib_path);

		if (!module_)
			throw E_LIB_NOT_FOUND;
	}

	library::~library()
	{
		FreeLibrary(module_);
	}

	f_export library::load_function(const char* function_name)
	{
		//std::unique_lock<std::mutex> lock(lib_lock_);
		f_export function = functions_[function_name];

		if (!function)
		{
			function = (f_export)GetProcAddress(module_, function_name);

			if (!function)
				throw E_FUNC_NOT_FOUND;

			functions_[function_name] = function;
		}

		return function;
	}
}