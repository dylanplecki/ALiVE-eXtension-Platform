
// Program Headers
#include "stdafx.h"
#include "session.h"
#include "library.h"

// STD Headers
#include <string>
#include <unordered_map>

// Windows Headers
#include <concurrent_queue.h>

namespace adp
{
	struct session::internals
	{
		bool active_;
		size_t handler_id_counter_;

		std::unordered_map<std::string, library> loaded_lib_map_;

		void call_lib_function(char* function, char* lib_path);
		void get_chunk(char* address);
	};


}