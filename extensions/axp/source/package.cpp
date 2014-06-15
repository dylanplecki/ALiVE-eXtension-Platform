
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
		source_.append(source_data, copy_length);
		source_.replace(source_.begin(), source_.end(), PORTION_DELIMITER, 0);
		
		const char* source_ptr = source_.c_str();
		size_t source_len = source_.length() - 1;

		for (size_t i = 0; i < source_len; ++i)
		{
			if (!source_ptr[i]) // If null
				portions_.push_back(source_ptr + (i + 1));
		}
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
	}

	size_t package::source_size()
	{
		return source_size_;
	}

	const char* package::read_source()
	{
		return source_.c_str();
	}
	
	std::vector<const char*>& package::source_portions()
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
