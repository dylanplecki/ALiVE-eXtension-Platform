#pragma once

// STD Headers
#include <string>
#include <unordered_map>

namespace adp
{
	class library; // Defined in <src/library.cpp>

	class session
	{
	private:
		bool active_;
		size_t handler_id_counter_;

		std::unordered_map<std::string, library> loaded_lib_map_;

		void run_lib_function(const char* lib_path, const char* function, const bool &async);
		void get_chunk(const char* address);

	public:
		session();
		~session();

		bool active();
		void process_input(char* output_buffer, int outputSize, const char* input_data);
	};
}
