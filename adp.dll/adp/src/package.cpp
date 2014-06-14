
// Program Headers
#include <adp/stdafx.h>
#include <adp/package.h>

// STD Headers
#include <algorithm>

namespace adp
{
	package::package(const char* source_data) : source_(source_data), copy_(false)
	{
	}

	package::package(const char* source_data, const size_t &source_size) : copy_(true)
	{
		std::copy(source_data, (source_data + source_size), source_copy_);
	}

	package::~package()
	{
		std::lock_guard<std::mutex> lk(lock_);
		if (source_)
			delete source_;
		if (source_copy_)
			delete source_copy_;
	}

	const char* package::read_source()
	{
		if (copy_)
			return source_copy_;
		else
			return source_;
	}
	
	void package::write_sink(const char* input_data)
	{
		std::lock_guard<std::mutex> lk(lock_);
		sink_.append(input_data);
	}

	size_t package::flush_sink(size_t buffer_size, char* output_buffer)
	{
		std::lock_guard<std::mutex> lk(lock_);
		const char* sink_ptr = sink_.c_str();

		if (buffer_size > sink_.size())
			buffer_size = sink_.size();

		if (buffer_size)
		{
			std::copy(sink_ptr, (sink_ptr + buffer_size), output_buffer);

			if (buffer_size == sink_.size())
				sink_.clear();
			else
				sink_ = sink_.substr(buffer_size, std::string::npos);
		}

		return sink_.size();
	}
}
