#pragma once

// Program Headers
#include <aep/logger.h>

// Boost Headers
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

#define AEP_LOG_STREAM_WITH_PARAMS(params_seq) \
	BOOST_LOG_STREAM_WITH_PARAMS(aep::logger::global_logger, (boost::log::keywords::channel = "global")params_seq)

#define AEP_LOG_STREAM \
	AEP_LOG_STREAM_WITH_PARAMS()

#define AEP_LOG_STREAM_SEV(lvl) \
	AEP_LOG_STREAM_WITH_PARAMS((boost::log::keywords::severity = aep::logger::lvl))

namespace aep
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
