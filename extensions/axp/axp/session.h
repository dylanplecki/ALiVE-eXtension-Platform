#pragma once

// Program Headers
#include <axp/library.h>

// STD Headers
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <queue>
#include <thread>

// Boost Headers
#include <boost/utility/string_ref.hpp>

// Platform Specific Defines
#if defined(_WIN32) || defined(_WIN64)
	#define DYNAMIC_LIBRARY_PATH "extensions\\"
	#define DYNAMIC_LIBRARY_EXT ".dll"
#elif defined(__linux)
	#define DYNAMIC_LIBRARY_PATH "extensions/"
	#define DYNAMIC_LIBRARY_EXT ".so"
#else
	#error Cannot load dynamic library path: OS not supported
#endif


namespace axp
{
	class package; // Defined in <src/package.cpp>

	class session
	{
	private:
		bool active_;

		std::mutex lib_lock_;
		std::mutex queue_lock_;
		std::mutex storage_lock_;

		std::queue<std::shared_ptr<package>> package_output_queue_;
		std::unordered_map<std::uintptr_t, std::shared_ptr<package>> package_output_storage_;
		std::unordered_map<std::string, std::shared_ptr<library>> loaded_lib_map_;

		void add_to_storage(const std::shared_ptr<package> &package_ref);
		void remove_from_storage(const std::uintptr_t &package_ptr_int);
		void remove_from_storage(package* package_ptr);
		void remove_from_storage(const std::shared_ptr<package> &package_ref);

		f_export pull_lib_function(boost::string_ref data_in);

		void export_address(void* ptr_address, char* output_buffer);
		char export_chunk(const char* chunk_addr_str, const size_t &output_size, char* output_buffer);
		char export_package(const std::shared_ptr<package> &output_package, size_t output_size, char* output_buffer);
		char export_next_package(const size_t &output_size, char* output_buffer);

	public:
		session();
		~session();

		bool active();
		void set_active(const bool &status);
		void queue_output(const std::shared_ptr<package> &package_handle);
		void process_input(const char &input_status, const char* input_data, char &output_status, int output_size, char* output_buffer);
	};
}
