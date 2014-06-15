
// Program Headers
#include <axp/stdafx.h>
#include <axp/logger.h>
#include <axp/logger_boost.h>

// Boost Headers
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace axp
{
	namespace logger
	{
		logger_type global_logger(boost::log::keywords::severity = info);

		void initialize(const char* log_file, const char* log_format, const severity_level &log_level)
		{
			boost::log::register_simple_formatter_factory<severity_level, char>("Severity");
			boost::log::add_file_log(
				boost::log::keywords::file_name = log_file,
				boost::log::keywords::format = log_format,
				boost::log::keywords::auto_flush = true
			);
			boost::log::core::get()->set_filter(
				severity >= log_level
			);
			boost::log::add_common_attributes();
		}
	}
}
