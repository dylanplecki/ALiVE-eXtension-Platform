#pragma once

// Program Headers
#include <axp\sqf_object.h>

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
		sqf::variable sqf_data_;

	public:
		package(const char* source_data, const size_t &copy_length);
		~package();

		size_t source_size() const;
		const char* read_source() const;

		const sqf::variable& sqf_data() const;

		size_t sink_size() const;
		void write_sink(const char* input_data);
		size_t flush_sink(size_t buffer_size, char* output_buffer, char** end_ptr); // Returns characters left
	};
}