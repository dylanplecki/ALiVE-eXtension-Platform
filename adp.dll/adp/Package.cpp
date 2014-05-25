#include "stdafx.h" // Precompiled headers

#include <boost/algorithm/string.hpp>

#include "Package.h"


// Message protocol specification
// (should be in +1 inc order)
#define STATUS_POS 0
#define FUNCTION_POS 1
#define MESSAGE_START_POS 2


ADP::Package::Package(const boost::string_ref &full_message) : full_message_(full_message)
{
	{ // Protect package id increment from non-atomic operations
		std::lock_guard<std::mutex> lock(package_static_var_mutex_);
		package_id_ = package_id_inc_;
		++package_id_inc_;
		++package_active_count_;
	}

	if (full_message.size() < 3)
		throw E_PACKAGE_MESSAGE_TOO_SHORT;

	// Get message parameters
	status_ = full_message[STATUS_POS];
	function_address_ = full_message[FUNCTION_POS];

	// Split arguments
	boost::split(arguments_, full_message.substr(MESSAGE_START_POS, boost::string_ref::npos), 31); // [UTF-8] Unit separator (UTF+001F &#31)
}


ADP::Package::~Package()
{
	std::lock_guard<std::mutex> lock(package_static_var_mutex_);
	--package_active_count_;
}


bool ADP::Package::GetResult(size_t buffer_size, char* buffer)
{
	buffer_size -= 1;
	result_buffer_.copy(buffer, buffer_size, result_buffer_pos_);
	result_buffer_pos_ += buffer_size;
	buffer[buffer_size + 1] = '\0'; // Escape with null end
	return (result_buffer_pos_ < result_buffer_.size());
}
