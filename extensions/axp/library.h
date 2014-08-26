#pragma once

// STD Headers
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>


// General Defines
#define F_EXPORT_HANDLER_PTR_T axp::handler*

// Platform Specific Defines
#if defined(_WIN32) || defined(_WIN64)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#define EXPORT_CALL_TYPE __stdcall
	#define DYN_LIB_HANDLE_TYPE HINSTANCE
#else
	#define EXPORT_CALL_TYPE
	#define DYN_LIB_HANDLE_TYPE void*
#endif

// Exception Codes
#define E_LIB_NOT_FOUND 0
#define E_FUNC_NOT_FOUND 1


namespace axp
{
	class handler; // Defined in <src/handler.cpp>

	typedef void(EXPORT_CALL_TYPE *f_export)(F_EXPORT_HANDLER_PTR_T);
	typedef std::unordered_map<std::string, f_export> function_lookup_list;

	class library
	{
	private:
		const std::string lib_path_;
		std::mutex lib_lock_;
		DYN_LIB_HANDLE_TYPE module_;
		function_lookup_list functions_;

	public:
		library(const std::string &lib_name);
		~library();
		f_export load_function(const char* function_name);
	};
}
