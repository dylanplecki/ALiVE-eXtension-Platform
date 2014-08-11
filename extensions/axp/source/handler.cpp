
// Program Headers
#include <axp/stdafx.h>
#include <axp/handler.h>
#include <axp/session.h>
#include <axp/package.h>
#include <axp/logger_boost.h>

// STD Headers
#include <mutex>


#define axp_HANDLER_LOG_MSG_WITH_PARAMS(logger, message, params_seq) \
	boost::log::record rec = logger.open_record((BOOST_PP_SEQ_ENUM((boost::log::keywords::channel = "handlers")params_seq))); \
	if (rec) \
	{ \
		boost::log::record_ostream strm(rec); \
		strm << message; \
		strm.flush(); \
		logger.push_record(boost::move(rec)); \
	}

#define axp_HANDLER_LOG_MSG(logger, message) \
	axp_HANDLER_LOG_MSG_WITH_PARAMS(logger, message,)


namespace axp
{
	extern std::string current_lib_path; // Defined in <axp/axp.h>


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


	size_t handler::input_size() const
	{
		return internals_->package_handle_->source_size();
	}


	const char* handler::input_source() const
	{
		return internals_->package_handle_->read_source();
	}


	const sqf::variable& handler::input_data() const
	{
		return internals_->package_handle_->sqf_data();
	}


	bool handler::check_async() const
	{
		return internals_->async_;
	}


	bool handler::active_session() const
	{
		return internals_->session_handle_->active();
	}


	const std::string& handler::working_directory() const
	{
		return current_lib_path;
	}


	void handler::log(const char* message) const
	{
		axp_HANDLER_LOG_MSG(internals_->logger_, message);
	}


	void handler::log(const char* message, const logger::severity_level &severity) const
	{
		axp_HANDLER_LOG_MSG_WITH_PARAMS(internals_->logger_, message, (boost::log::keywords::severity = severity));
	}


	void handler::export_data(const char* output_data) const
	{
		std::lock_guard<std::mutex> lock(internals_->main_lock_);

		internals_->package_handle_->write_sink(output_data);

		if (internals_->async_)
			internals_->session_handle_->queue_output(internals_->package_handle_);
	}


	void handler::export_data(const std::string &output_data) const
	{
		export_data(output_data.c_str());
	}


	void handler::export_data(const sqf::variable& output_data) const
	{
		export_data(output_data.to_string().c_str());
	}


	void handler::attach_thread(std::thread* new_thread) const
	{
		attach_thread(std::shared_ptr<std::thread>(new_thread));
	}


	void handler::attach_thread(const std::shared_ptr<std::thread> &new_thread) const
	{
		std::lock_guard<std::mutex> lock(internals_->main_lock_);
		internals_->attached_threads_.push_back(new_thread);
	}
}
