#pragma once

// Program Headers
#include <axp/linkages.h>
#include <axp/logger.h>
#include <axp/shared_ptr.h>
#include <axp/sqf_object.h>

// STD Headers
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>


namespace axp
{
	class package; // Defined in <src/package.cpp>
	class session; // Defined in <src/session.cpp>

	class LibLink handler
	{
	private:
		bool async_;
		std::mutex main_lock_;
		axp::shared_ptr<session> session_handle_;
		axp::shared_ptr<package> package_handle_;

	public:
		handler(session* parent_session, package* call_package, bool async);
		~handler();

		size_t input_size() const;
		const char* input_source() const;
		const sqf::variable& input_data() const;

		bool check_async() const;
		bool active_session() const;
		const std::string& working_directory() const;

		void log(const char* message) const;
		void log(const char* message, const logger::severity_level &severity) const;

		void export_data(const char* output_data);
		void export_data(const std::string &output_data);
		void export_data(const sqf::variable &output_data);
	};
}
