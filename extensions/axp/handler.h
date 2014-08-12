#pragma once

// Program Headers
#include <axp/linkages.h>
#include <axp/logger.h>
#include <axp/sqf_object.h>

// STD Headers
#include <memory>
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
		struct internals;
		std::shared_ptr<internals> internals_;

	public:
		handler(const std::shared_ptr<session> &parent_session, const std::shared_ptr<package> &call_package, const bool &async);
		~handler();

		size_t input_size() const;
		const char* input_source() const;
		const sqf::variable& input_data() const;

		bool check_async() const;
		bool active_session() const;
		const std::string& working_directory() const;

		void log(const char* message) const;
		void log(const char* message, const logger::severity_level &severity) const;

		void export_data(const char* output_data) const;
		void export_data(const std::string &output_data) const;
		void export_data(const sqf::variable& output_data) const;

		void attach_thread(std::thread* new_thread) const;
		void attach_thread(const std::shared_ptr<std::thread> &new_thread) const;
	};
}
