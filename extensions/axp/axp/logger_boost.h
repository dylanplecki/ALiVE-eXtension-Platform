#pragma once

// Program Headers
#include <axp/logger.h>

// Boost Headers
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

#define AXP_LOG_STREAM_WITH_PARAMS(params_seq) \
	BOOST_LOG_STREAM_WITH_PARAMS(axp::logger::global_logger, (boost::log::keywords::channel = "global")params_seq)

#define AXP_LOG_STREAM \
	AXP_LOG_STREAM_WITH_PARAMS()

#define AXP_LOG_STREAM_SEV(lvl) \
	AXP_LOG_STREAM_WITH_PARAMS((boost::log::keywords::severity = axp::logger::lvl))

namespace axp
{
	namespace logger
	{
		typedef boost::log::sources::severity_channel_logger_mt<severity_level> logger_type;

		BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level);
		BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string);

		extern logger_type global_logger;

		void initialize(const char* log_file, const char* log_format, const severity_level &log_level);
	}
}
