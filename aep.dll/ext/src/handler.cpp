
// Program Headers
#include <aep/stdafx.h>
#include <aep/handler.h>
#include <aep/session.h>
#include <aep/package.h>
#include <aep/logger_boost.h>

// STD Headers
#include <mutex>

#define AEP_HANDLER_LOG_MSG_WITH_PARAMS(logger, message, params_seq) \
	boost::log::record rec = logger.open_record((BOOST_PP_SEQ_ENUM((boost::log::keywords::channel = "handlers")params_seq))); \
	if (rec) \
	{ \
		boost::log::record_ostream strm(rec); \
		strm << message; \
		strm.flush(); \
		logger.push_record(boost::move(rec)); \
	}

#define AEP_HANDLER_LOG_MSG(logger, message) \
	AEP_HANDLER_LOG_MSG_WITH_PARAMS(logger, message,)

namespace aep
{
	extern std::string current_lib_path; // Defined in <aep/aep.h>

	struct handler::internals
	{
		bool async_;
		std::mutex main_lock_;
		logger::logger_type logger_;
		std::shared_ptr<session> session_handle_;
		std::shared_ptr<package> package_handle_;
		std::vector<std::shared_ptr<std::thread>> attached_threads_;
	};

	handler::handler(const std::shared_ptr<session> &parent_session, const std::shared_ptr<package> &call_package, const bool &async = false) : internals_(new internals)
	{
		internals_->async_ = async;
		internals_->session_handle_ = parent_session;
		internals_->package_handle_ = call_package;
	}

	handler::~handler()
	{
	}

	size_t handler::input_size()
	{
		internals_->package_handle_->source_size();
	}

	const char* handler::input_data()
	{
		internals_->package_handle_->read_source();
	}

	std::vector<char*>& handler::arguments()
	{
		return internals_->package_handle_->source_portions();
	}

	bool handler::check_async()
	{
		return internals_->async_;
	}

	bool handler::active_session()
	{
		return internals_->session_handle_->active();
	}

	const std::string& handler::working_directory()
	{
		return current_lib_path;
	}

	void handler::log(const char* message)
	{
		AEP_HANDLER_LOG_MSG(internals_->logger_, message);
	}

	void handler::log(const char* message, const logger::severity_level &severity)
	{
		AEP_HANDLER_LOG_MSG_WITH_PARAMS(internals_->logger_, message, (boost::log::keywords::severity = severity));
	}

	void handler::export_data(const char* output_data)
	{
		std::lock_guard<std::mutex> lock(internals_->main_lock_);

		internals_->package_handle_->write_sink(output_data);

		if (internals_->async_)
			internals_->session_handle_->queue_output(internals_->package_handle_);
	}

	void handler::attach_thread(std::thread* new_thread)
	{
		attach_thread(std::shared_ptr<std::thread>(new_thread));
	}

	void handler::attach_thread(const std::shared_ptr<std::thread> &new_thread)
	{
		std::lock_guard<std::mutex> lock(internals_->main_lock_);
		internals_->attached_threads_.push_back(new_thread);
	}
}
