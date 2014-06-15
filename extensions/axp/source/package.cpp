
// Program Headers
#include <axp/stdafx.h>
#include <axp/package.h>

// STD Headers
#include <algorithm>

// Boost Headers
#include <boost/utility/string_ref.hpp>

namespace axp
{
	void package::init(const char* source_data, const size_t &copy_length)
	{
		source_size_ = copy_length;
		source_ = new char[copy_length + 1];
		std::copy(source_data, (source_data + copy_length), source_);

		size_t i = 0;
		for (; i < copy_length; ++i)
		{
			if (!source_[i]) break; // Break at first null
			if (source_[i] == PORTION_DELIMITER)
			{
				source_[i] = 0; // Null terminator
				portions_.push_back(source_ + (i + 1));
			}
		}

		if (source_[i]) // Ending isn't null
			source_[i + 1] = 0; // Add null ending
	}

	package::package(const char* source_data)
	{
		init(source_data, strlen(source_data));
	}

	package::package(const char* source_data, const size_t &copy_length)
	{
		init(source_data, copy_length);
	}

	package::~package()
	{
		std::lock_guard<std::mutex> lk(lock_);
		delete source_;
	}

	size_t package::source_size()
	{
		return source_size_;
	}

	const char* package::read_source()
	{
		return source_;
	}
	
	std::vector<char*>& package::source_portions()
	{
		return portions_;
	}

	void package::write_sink(const char* input_data)
	{
		std::lock_guard<std::mutex> lk(lock_);

		if (sink_.size()) // Comma-delimited sink
			sink_.push_back(',');

		sink_.append(input_data);
	}

	size_t package::sink_size()
	{
		return sink_.size();
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

			std::copy(sink_ptr, (sink_ptr + buffer_size), output_buffer);

			if (end_ptr)
				*end_ptr = output_buffer + buffer_size;

			if (buffer_size == sink_size)
				sink_.clear();
			else
				sink_ = sink_.substr(buffer_size, std::string::npos);

			return sink_.size();
		}
		return sink_size;
	}
}
