
// Program Headers
#include <aep/stdafx.h>
#include <aep/logger.h>
#include <aep/logger_boost.h>

// Boost Headers
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace aep
{
	namespace logger
	{
		logger_type global_logger(boost::log::keywords::severity = info, boost::log::keywords::channel = "global");

		void initialize(const char* log_file, const char* log_format, const severity_level &log_level)
		{
			boost::log::register_simple_formatter_factory<severity_level, char>("Severity");
			boost::log::add_file_log(
				boost::log::keywords::file_name = log_file,
				boost::log::keywords::format = log_format,
				boost::log::keywords::auto_flush = true
			);
			boost::log::core::get()->set_filter(
				severity >= log_level && channel == "global"
			);
			boost::log::add_common_attributes();
		}

		boost::log::record_ostream& global()
		{
			return boost::log::aux::make_record_pump(global_logger, global_logger.open_record()).stream();
		}

		boost::log::record_ostream& global(const severity_level &severity)
		{
			return boost::log::aux::make_record_pump(global_logger, global_logger.open_record()).stream();
		}

		void global(const char* message)
		{
			global() << message;
		}

		void global(const char* message, const severity_level &severity)
		{
			global(severity) << message;
		}
	}
}
