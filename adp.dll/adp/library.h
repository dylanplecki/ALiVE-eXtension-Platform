#pragma once

#define WIN32_LEAN_AND_MEAN

// Program Headers
#include "adp.h"

// STD Headers
#include <string>
#include <unordered_map>

// Windows Headers
#include <Windows.h>

namespace adp
{
	class library
	{
	private:
		const HINSTANCE module_;
		std::unordered_map<std::string, f_export> functions_;

	public:
		library(HINSTANCE module);
		~library();
		HINSTANCE get_module();
		std::unordered_map<std::string, f_export> get_functions();
	};
}
