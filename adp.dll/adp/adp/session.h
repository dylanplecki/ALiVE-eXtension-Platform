#pragma once

// Program Headers
#include <adp/library.h>

// STD Headers
#include <memory>
#include <string>
#include <unordered_map>
#include <queue>

namespace adp
{
	class package; // Defined in <src/package.cpp>

	class session
	{
	private:
		bool active_;

		std::queue<std::shared_ptr<package>> package_output_queue_;
		std::unordered_map<package*, std::shared_ptr<package>> package_output_storage_;
		std::unordered_map<std::string, library> loaded_lib_map_;

		void run_lib_function(const char* lib_path, const char* function, const bool &async);

	public:
		session();
		~session();

		bool active();
		void process_input(const char* input_data, int outputSize, char* output_buffer);
		void queue_output(const std::shared_ptr<package> &package_handle);
	};
}
