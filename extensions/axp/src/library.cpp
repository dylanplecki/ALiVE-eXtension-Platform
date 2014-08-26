
// Program Headers
#include <axp/stdafx.h>
#include <axp/library.h>
#include <axp/logger_boost.h>

// Boost Headers
#include <boost/filesystem.hpp>

#if defined(__linux)
	#include <dlfcn.h>
#endif


// Defines
#define DYNAMIC_LIBRARY_FOLDER "extensions"

// Platform Specific Defines
#if defined(_WIN32) || defined(_WIN64)
	#define DYNAMIC_LIBRARY_EXT ".dll"
#elif defined(__linux)
	#define DYNAMIC_LIBRARY_EXT ".so"
#else
	#error Cannot load dynamic library path: OS not supported
#endif


namespace axp
{
	extern std::string current_lib_path;

	library::library(const std::string &lib_name)
	{
		using namespace boost::filesystem;

		std::string lib_path("");
		path search_path(path(current_lib_path).parent_path());
		directory_iterator search_it(search_path);

		for (auto fs_obj : search_it)
		{
			path ext_path(fs_obj.path());
			ext_path /= DYNAMIC_LIBRARY_FOLDER;
			ext_path /= lib_name + DYNAMIC_LIBRARY_EXT;

			if (is_regular_file(ext_path))
			{
				lib_path = ext_path.string();
				break;
			}
		}

		if (lib_path.empty())
			throw E_LIB_NOT_FOUND;

		#if defined(_WIN32) || defined(_WIN64)
			wchar_t win_lib_path[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, lib_path.c_str(), -1, win_lib_path, MAX_PATH);
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
		std::unique_lock<std::mutex> lock(lib_lock_);
		f_export function = functions_[function_name];

		if (!function)
		{
			try
			{
				#if defined(_WIN32) || defined(_WIN64)
					
					// For WINAPI __stdcall
					std::string func_name_stdcall;
					func_name_stdcall.push_back('_');
					func_name_stdcall.append(function_name);
					func_name_stdcall.push_back('@');
					func_name_stdcall.append(std::to_string(sizeof(F_EXPORT_HANDLER_PTR_T)));
					
					function = reinterpret_cast<f_export>(GetProcAddress(module_, func_name_stdcall.c_str()));

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
