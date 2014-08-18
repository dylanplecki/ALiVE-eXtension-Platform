#pragma once

// Program Headers
#include <axp/linkages.h>
#include <axp/logger.h>
#include <axp/protocol_def.h>
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

	class handler
	{
	private:
		size_t exec_type_;
		std::mutex main_lock_;
		axp::shared_ptr<session> session_handle_;
		axp::shared_ptr<package> package_handle_;

	public:
		LibLink handler(session* parent_session, package* call_package, const size_t &exec_type);
		LibLink ~handler();

		LibLink size_t input_size() const;
		LibLink const char* input_source() const;
		LibLink const sqf::variable& input_data() const;

		LibLink size_t exec_type() const;
		LibLink bool active_session() const;
		LibLink const std::string& working_directory() const;

		LibLink void log(const char* message) const;
		LibLink void log(const char* message, const logger::severity_level &severity) const;

		LibLink void export_data(const char* output_data);
		LibLink void export_data(const std::string &output_data);
		LibLink void export_data(const sqf::variable &output_data);
	};
}
