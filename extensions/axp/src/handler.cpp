
// Program Headers
#include <axp/stdafx.h>
#include <axp/handler.h>
#include <axp/session.h>
#include <axp/package.h>
#include <axp/logger_boost.h>


#define AXP_HANDLER_LOG_MSG_WITH_PARAMS(logger, message, params_seq) \
	boost::log::record rec = logger.open_record((BOOST_PP_SEQ_ENUM((boost::log::keywords::channel = "handlers")params_seq))); \
	if (rec) \
	{ \
		boost::log::record_ostream strm(rec); \
		strm << message; \
		strm.flush(); \
		logger.push_record(boost::move(rec)); \
	}

#define AXP_HANDLER_LOG_MSG(logger, message) \
	AXP_HANDLER_LOG_MSG_WITH_PARAMS(logger, message, )


namespace axp
{
	extern std::string current_lib_path; // Defined in <axp/axp.h>


	handler::handler(session* parent_session, package* call_package, const size_t &exec_type = SF_SYNC) : exec_type_(exec_type), session_handle_(parent_session), package_handle_(call_package)
	{
	}


	handler::~handler()
	{
		//main_lock_.lock();
	}


	size_t handler::input_size() const
	{
		return package_handle_->source_size();
	}


	const char* handler::input_source() const
	{
		return package_handle_->read_source();
	}


	const sqf::variable& handler::input_data() const
	{
		return package_handle_->sqf_data();
	}


	size_t handler::exec_type() const
	{
		return exec_type_;
	}


	bool handler::active_session() const
	{
		return session_handle_->active();
	}


	const std::string& handler::working_directory() const
	{
		return current_lib_path;
	}


	void handler::log(const char* message) const
	{
		AXP_HANDLER_LOG_MSG(logger::global_logger, message);
	}


	void handler::log(const char* message, const logger::severity_level &severity) const
	{
		AXP_HANDLER_LOG_MSG_WITH_PARAMS(logger::global_logger, message, (boost::log::keywords::severity = severity));
	}


	void handler::export_data(const char* output_data)
	{
		std::lock_guard<std::mutex> lock(main_lock_);

		package_handle_->write_sink(output_data);

		if (exec_type_ == SF_ASYNC || exec_type_ == SF_SYNC_SR)
			session_handle_->queue_output(package_handle_);
	}


	void handler::export_data(const std::string &output_data)
	{
		export_data(output_data.c_str());
	}


	void handler::export_data(const sqf::variable& output_data)
	{
		export_data(output_data.to_string().c_str());
	}
}
