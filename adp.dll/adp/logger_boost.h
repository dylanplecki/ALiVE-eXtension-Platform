#pragma once

// Program Headers
#include "logger.h"

// Boost Headers
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

// Macros
#define ADP_LOG_STREAM adp::logger::global()
#define ADP_LOG_STREAM_SEV(lvl) adp::logger::global(adp::logger::lvl)

namespace adp
{
	namespace logger
	{
		typedef boost::log::sources::severity_channel_logger_mt<severity_level> logger_type;

		BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level);
		BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string);

		logger_type global_logger(boost::log::keywords::severity = info, boost::log::keywords::channel = "global");

		boost::log::record_ostream& global();
		boost::log::record_ostream& global(const severity_level &severity);
	}
}
