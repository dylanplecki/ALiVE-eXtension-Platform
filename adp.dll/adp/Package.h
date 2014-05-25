#pragma once

#define E_PACKAGE_MESSAGE_TOO_SHORT 0

class Package
{
private:
	uint32_t package_id_;

	static std::mutex package_static_var_mutex_;
	static uint32_t package_id_inc_;
	static uint32_t package_active_count_;

	uint8_t status_;
	uint8_t function_address_;

	boost::string_ref full_message_;
	std::vector<boost::string_ref> arguments_;

	std::string result_buffer_;
	size_t result_buffer_pos_;

public:
	Package(const boost::string_ref &full_message);
    ~Package();

	// Functions
	bool GetResult(size_t buffer_size, char* buffer);

	// Accessors
	uint32_t package_id() { return package_id_; };
	uint32_t package_active_count() { return package_active_count_; };
	uint8_t status() { return status_; };
	uint8_t function_address() { return function_address_; };
	boost::string_ref full_message() { return full_message_; };
	std::vector<boost::string_ref> arguments() { return arguments_; };
	const std::string& result_buffer() { return result_buffer_; };

	// Setters
	void set_result(const std::string& result_buffer) { result_buffer_ = result_buffer; };
};

