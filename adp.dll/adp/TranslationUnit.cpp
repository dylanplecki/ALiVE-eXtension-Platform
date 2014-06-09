#include "stdafx.h" // Precompiled headers

#include "TranslationUnit.h"


bool ADP::TranslationUnit::PackReturn(const char &status, const char &func, const boost::string_ref &message, std::stringstream &output_stream)
{
	return (bool)(output_stream << status << func << message);
}


bool ADP::TranslationUnit::PackReturn(const char &status, const char &func, Package* &a_package, const size_t &buffer_size, std::stringstream &output_stream)
{
	output_stream << status << func;
	return (bool)(a_package->GetResult((buffer_size - 2), output_stream));
}


bool ADP::TranslationUnit::ReturnError(const boost::string_ref &message, std::stringstream &output_stream)
{
	BOOST_LOG_TRIVIAL(warning) << "TranslationUnit: " << message;
	return PackReturn(SF_ERROR, FA_NONE, message, output_stream);
}


void ADP::TranslationUnit::ReturnPackage(ADP::Package* a_package, const size_t &buffer_size, std::stringstream &output_stream)
{
	char status = SF_EOF;

	if (a_package->current_result_size() > buffer_size) // Chunked output
		status = SF_CHUNK;

	output_stream << status << (char)FA_NONE;

	if (a_package->GetResult(buffer_size, output_stream))
	{
		std::lock_guard<std::mutex> lock(export_queue_lock_);

		// Add package to waiting exports container
		if (!waiting_exports_.count(a_package->package_id()))
			waiting_exports_[a_package->package_id()] = a_package;
	}
	else // Output finished
	{
		{ // Lock when processing export queue and map
			std::lock_guard<std::mutex> lock(export_queue_lock_);

			std::unordered_map<uint32_t, ADP::Package*>::iterator package_it = waiting_exports_.find(a_package->package_id());

			// Remove package from waiting exports container if found
			if (package_it != waiting_exports_.end())
				waiting_exports_.erase(package_it);
		}

		BOOST_LOG_TRIVIAL(trace) << "TranslationUnit: Finished returning package output to RVE, pid " << std::to_string(a_package->package_id());

		// Remove package
		delete a_package;
	}
}


void ADP::TranslationUnit::Import(const boost::string_ref &data, const size_t &buffer_size, std::stringstream &output_stream)
{
	ADP::Package* call_package = new ADP::Package(data);

	if (data[STATUS_POS] == SF_ASYNC) // Process in new thread
	{
		// TODO
	}
	else // Process synchronously in current thread
	{
		CallFunction(call_package);
		ReturnPackage(call_package, buffer_size, output_stream);
		return;
	}
}


void ADP::TranslationUnit::Receive(const char* &data, size_t buffer_size, char* &output_buffer)
{
	buffer_size -= 2;

	boost::string_ref data_wrap(data);
	std::stringstream output_stream;

	output_stream.rdbuf()->pubsetbuf(output_buffer, sizeof(output_buffer));

	if (data_wrap.size() > MESSAGE_START_POS)
	{
		switch (data[STATUS_POS])
		{
		case SF_NONE: // Return next package, if available
			Export(buffer_size, output_stream);
			break;
		case SF_CHUNK: // Find and return next chunk of package
			// TODO
			break;
		case SF_SYNC: case SF_ASYNC: // Run function with message
			Import(data_wrap, buffer_size, output_stream);
			break;
		default: // Return invalid status
			ReturnError("Invalid arguments: Status not valid.", output_stream);
			return;
		}
	}
	else
	{ // Error
		ReturnError("Invalid arguments: Message length did not meet minimum requirements.", output_stream);
		return;
	};
}


ADP::TranslationUnit::TranslationUnit()
{
	BOOST_LOG_TRIVIAL(trace) << "Creating new TranslationUnit object";
}


ADP::TranslationUnit::~TranslationUnit()
{
	BOOST_LOG_TRIVIAL(trace) << "Deleting a TranslationUnit object";
}
