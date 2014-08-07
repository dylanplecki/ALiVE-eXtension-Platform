
// Program Headers
#include <axp/stdafx.h>
#include <axp/package.h>

// STD Headers
#include <algorithm>

// Boost Headers
#include <boost/utility/string_ref.hpp>


namespace axp
{
	package::package(const char* source_data, const size_t &copy_length) : source_size_(copy_length), source_(source_data, copy_length), sqf_data_(source_)
	{
	}


	package::~package()
	{
	}


	size_t package::source_size() const
	{
		return source_size_;
	}


	const char* package::read_source() const
	{
		return source_.c_str();
	}


	const sqf::variable& package::sqf_data() const
	{
		return sqf_data_;
	}


	size_t package::sink_size() const
	{
		return sink_.size();
	}


	void package::write_sink(const char* input_data)
	{
		std::lock_guard<std::mutex> lk(lock_);

		if (sink_.size()) // Comma-delimited sink
			sink_.push_back(',');

		sink_.append(input_data);
	}


	size_t package::flush_sink(size_t buffer_size, char* output_buffer, char** end_ptr)
	{
		std::lock_guard<std::mutex> lk(lock_);
		size_t sink_size = sink_.size();

		if (sink_size && buffer_size)
		{
			const char* sink_ptr = sink_.c_str();

			if (buffer_size > sink_size)
				buffer_size = sink_size;
			else
				--buffer_size;

			std::copy(sink_ptr, (sink_ptr + buffer_size), output_buffer);
			output_buffer[buffer_size] = '\0';

			if (end_ptr)
				*end_ptr = output_buffer + buffer_size + 1;

			if (buffer_size == sink_size)
				sink_.clear();
			else
				sink_ = sink_.substr(buffer_size, std::string::npos);

			return sink_.size();
		}
		return sink_size;
	}
}
