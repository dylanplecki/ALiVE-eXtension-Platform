#pragma once

// STD Headers
#include <string>


namespace adp
{
	class session
	{
	private:
		struct internals;
		internals* internal_;

	public:
		session();
		~session();

		void process_input(char* output_buffer, int outputSize, const char* input_data);

		// Public Session API Functions
		bool check_active();
		std::string working_directory();
		void log(char log_level, const std::string &message);
		void queue_output(size_t handler_id, char* output_data);
	};
}
