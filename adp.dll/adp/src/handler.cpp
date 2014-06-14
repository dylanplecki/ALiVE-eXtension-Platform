
// Program Headers
#include <adp/stdafx.h>
#include <adp/handler.h>
#include <adp/session.h>

// STD Headers
#include <mutex>

namespace adp
{
	struct handler::internals
	{
		std::mutex main_lock_;
		size_t handler_id_;
		session* session_;
	};

	handler::handler(session* parent_session, const size_t &handler_id) : internals_(new internals)
	{
		internals_->handler_id_ = handler_id;
		internals_->session_ = parent_session;
	}

	handler::~handler()
	{
		delete internals_;
	}

	bool handler::session_active()
	{
		return internals_->session_->active();
	}

	void handler::engine_export(const char* output_data)
	{

	}
}
