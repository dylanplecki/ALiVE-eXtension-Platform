#pragma once

// Macros
#define ADP_LOG_MSG(msg) adp::logger::global(msg)
#define ADP_LOG_MSG_SEV(lvl, msg) adp::logger::global(adp::logger::lvl, msg)

namespace adp
{
	namespace logger
	{
		enum severity_level
		{
			trace,
			debug,
			info,
			warning,
			error,
			fatal
		};

		void initialize(const char* log_file, const char* log_format, const severity_level &log_level);

		void global(const char* message);
		void global(const char* message, const severity_level &severity);
	}
}
