#pragma once

#define PORTION_DELIMITER 31 // Unit separator (UTF+001F &#31)

// STD Headers
#include <mutex>
#include <string>
#include <vector>

namespace axp
{
	class package
	{
	private:
		std::mutex lock_;
		std::string sink_;
		std::string source_;
		size_t source_size_;
		std::vector<const char*> portions_;

		void init(const char* source_data, const size_t &copy_length);

	public:
		package(const char* source_data);
		package(const char* source_data, const size_t &copy_length);
		~package();

		size_t source_size();
		const char* read_source();
		std::vector<const char*>& source_portions();
		void write_sink(const char* input_data);
		size_t sink_size();

		size_t flush_sink(size_t buffer_size, char* output_buffer, char** end_ptr); // Returns characters left
	};
}