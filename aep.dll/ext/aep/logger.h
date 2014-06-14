#pragma once

// Macros
#define AEP_LOG_MSG(msg) aep::logger::global(msg)
#define AEP_LOG_MSG_SEV(lvl, msg) aep::logger::global(adp::logger::lvl, msg)

namespace aep
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
