#include "stdafx.h" // Precompiled headers

#include <boost/algorithm/string.hpp>

#include "Package.h"


ADP::Package::Package(const boost::string_ref &full_message) : full_message_(full_message)
{
	BOOST_LOG_TRIVIAL(trace) << "Creating a new Package object";

	{ // Protect package id increment from non-atomic operations
		std::lock_guard<std::mutex> lock(package_static_var_mutex_);
		package_id_ = package_id_inc_;
		++package_id_inc_;
		++package_active_count_;
	}

	if (full_message.size() < 3)
	{
		BOOST_LOG_TRIVIAL(error) << "Invalid Arguments: Package object constructor received message which was too short.";
		throw E_PACKAGE_MESSAGE_TOO_SHORT;
	}

	// Get message parameters
	status_ = full_message[STATUS_POS];
	function_address_ = full_message[FUNCTION_POS];

	// Split arguments
	boost::split(arguments_, full_message.substr(MESSAGE_START_POS, boost::string_ref::npos), (char) 31); // [UTF-8] Unit separator (UTF+001F &#31)
}


ADP::Package::~Package()
{
	std::lock_guard<std::mutex> lock(package_static_var_mutex_);
	--package_active_count_;
	BOOST_LOG_TRIVIAL(trace) << "Deleting a Package object, left active count of " << std::to_string(package_active_count_);
}


bool ADP::Package::GetResult(const size_t &buffer_size, std::stringstream &output_stream)
{
	output_stream << result_buffer_.substr(result_buffer_pos_, buffer_size);
	result_buffer_pos_ += buffer_size;

	return (result_buffer_pos_ < result_buffer_.size());
}


bool ADP::Package::GetResult(const size_t &buffer_size, char* &buffer)
{
	std::stringstream output_stream;
	output_stream.rdbuf()->pubsetbuf(buffer, sizeof(buffer));

	return GetResult(buffer_size, output_stream);
}
