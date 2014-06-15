#pragma once

// Program Headers
#include <aep/library.h>

// STD Headers
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <queue>
#include <thread>

// Boost Headers
#include <boost/utility/string_ref.hpp>

#define DYNAMIC_LIBRARY_PATH "extensions\\"
#define DYNAMIC_LIBRARY_EXT ".dll"

namespace aep
{
	class package; // Defined in <src/package.cpp>

	class session
	{
	private:
		bool active_;

		std::mutex lib_lock_;
		std::mutex queue_lock_;

		std::queue<std::shared_ptr<package>> package_output_queue_;
		std::unordered_map<package*, std::shared_ptr<package>> package_output_storage_;
		std::unordered_map<std::string, std::shared_ptr<library>> loaded_lib_map_;

		f_export pull_lib_function(boost::string_ref data_in);
		char export_package(const std::shared_ptr<package> &output_package, const size_t &output_size, char* output_buffer);
		char export_next_package(const size_t &output_size, char* output_buffer);

	public:
		session();
		~session();

		bool active();
		void process_input(const char* input_data, int output_size, char* output_buffer);
		void queue_output(const std::shared_ptr<package> &package_handle);
	};
}
