#pragma once

// STD Headers
#include <mutex>
#include <string>

namespace adp
{
	class package
	{
	private:
		const bool copy_;
		std::mutex lock_;
		std::string sink_;
		char* source_copy_;
		const char* source_;

	public:
		package(const char* source_data); // Reference
		package(const char* source_data, const size_t &source_size); // Copy
		~package();

		const char* read_source();
		void write_sink(const char* input_data);

		size_t flush_sink(size_t buffer_size, char* output_buffer); // Returns characters left
	};
}