#pragma once

// Program Headers
#include <aep/logger.h>

// Boost Headers
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

// Macros
#define AEP_LOG_STREAM aep::logger::global()
#define AEP_LOG_STREAM_SEV(lvl) aep::logger::global(aep::logger::lvl)

namespace aep
{
	namespace logger
	{
		typedef boost::log::sources::severity_channel_logger_mt<severity_level> logger_type;

		BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level);
		BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string);

		extern logger_type global_logger;

		boost::log::record_ostream& global();
		boost::log::record_ostream& global(const severity_level &severity);
	}
}
