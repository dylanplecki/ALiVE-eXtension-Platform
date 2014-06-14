#pragma once

// STD Headers
#include <string>

namespace adp
{
	class session; // Defined in <src/session.cpp>

	class handler
	{
	private:
		struct internals;
		internals* internals_;

	public:
		handler(session* parent_session, const size_t &handler_id);
		~handler();

		bool session_active();
		void engine_export(const char* output_data);
	};
}
