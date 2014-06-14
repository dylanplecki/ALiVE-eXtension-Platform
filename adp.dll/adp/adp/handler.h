#pragma once

// STD Headers
#include <memory>
#include <string>

namespace adp
{
	class package; // Defined in <src/package.cpp>
	class session; // Defined in <src/session.cpp>

	class handler
	{
	private:
		struct internals;
		internals* internals_;

	public:
		handler(const std::shared_ptr<session> &parent_session, const std::shared_ptr<package> &call_package);
		~handler();

		bool active_session();
		void export_data(const char* output_data);
	};
}
