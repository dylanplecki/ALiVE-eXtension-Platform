#pragma once

// Program Headers
#include <axp/logger.h>

// STD Headers
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace axp
{
	class package; // Defined in <src/package.cpp>
	class session; // Defined in <src/session.cpp>

	class handler
	{
	private:
		struct internals;
		std::shared_ptr<internals> internals_;

	public:
		handler(const std::shared_ptr<session> &parent_session, const std::shared_ptr<package> &call_package, const bool &async);
		~handler();

		size_t input_size();
		const char* input_data();
		std::vector<const char*>& arguments();

		bool check_async();
		bool active_session();
		const std::string& working_directory();

		void log(const char* message);
		void log(const char* message, const logger::severity_level &severity);

		void export_data(const char* output_data);
		void attach_thread(std::thread* new_thread);
		void attach_thread(const std::shared_ptr<std::thread> &new_thread);
	};
}
