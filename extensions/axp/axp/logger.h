#pragma once

// STD Headers
#include <iostream>

namespace axp
{
	namespace logger
	{
		enum severity_level
		{
			error = 0,
			critical = 1,
			info = 2,
			warning = 3,
			debug = 4,
			trace = 5
		};

		std::ostream& operator<< (std::ostream& strm, severity_level level);
	}
}
