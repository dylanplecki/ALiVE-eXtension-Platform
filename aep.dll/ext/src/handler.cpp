
// Program Headers
#include <aep/stdafx.h>
#include <aep/handler.h>
#include <aep/session.h>
#include <aep/package.h>

// STD Headers
#include <mutex>
#include <vector>

namespace aep
{
	struct handler::internals
	{
		std::mutex main_lock_;
		size_t handler_id_;
		std::shared_ptr<session> session_handle_;
		std::shared_ptr<package> package_handle_;
	};

	handler::handler(const std::shared_ptr<session> &parent_session, const std::shared_ptr<package> &call_package) : internals_(new internals)
	{
		internals_->session_handle_ = parent_session;
		internals_->package_handle_ = call_package;
	}

	handler::~handler()
	{
		std::lock_guard<std::mutex> lock(internals_->main_lock_);
		delete internals_;
	}

	bool handler::active_session()
	{
		std::lock_guard<std::mutex> lock(internals_->main_lock_);
		return internals_->session_handle_->active();
	}

	void handler::export_data(const char* output_data)
	{
		std::lock_guard<std::mutex> lock(internals_->main_lock_);
		internals_->package_handle_->write_sink(output_data);
		internals_->session_handle_->queue_output(internals_->package_handle_);
	}
}
