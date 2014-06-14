#pragma once

// STD Headers
#include <mutex>
#include <string>
#include <unordered_map>

#define WIN32_LEAN_AND_MEAN

// Windows Headers
#include <Windows.h>

// Exception Codes
#define E_LIB_NOT_FOUND 0
#define E_FUNC_NOT_FOUND 1

namespace adp
{
	class handler; // Defined in <src/handler.cpp>

	/*
	Export function prototype:
	void __declspec(dllexport) __stdcall MyExportedFunction(adp::handler* call_handler);
	*/
	typedef void(__stdcall *f_export)(adp::handler*);
	typedef std::unordered_map<std::string, f_export> function_lookup_list;

	class library
	{
	private:
		const std::string lib_path_;
		std::mutex lib_lock_;
		HINSTANCE module_;
		function_lookup_list functions_;

	public:
		library(const char* lib_path);
		~library();
		f_export load_function(const char* function_name);
	};
}
