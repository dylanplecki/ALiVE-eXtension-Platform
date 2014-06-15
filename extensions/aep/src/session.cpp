
// Program Headers
#include <aep/stdafx.h>
#include <aep/session.h>
#include <aep/package.h>
#include <aep/handler.h>
#include <aep/version.h>
#include <aep/logger_boost.h>
#include <aep/protocol_def.h>

namespace aep
{
	extern std::string current_lib_path; // Defined in <aep/aep.cpp>

	f_export session::pull_lib_function(boost::string_ref data_in)
	{
		size_t delim_pos;
		std::string lib_name;
		std::string func_name;
		unsigned char i = 0;

		do {
			delim_pos = data_in.find_first_of(31);
			if (delim_pos == boost::string_ref::npos) break;
			switch (i)
			{
			case 0:
				lib_name = data_in.substr(0, delim_pos).data();
				break;
			case 1:
				func_name = data_in.substr(0, delim_pos).data();
				break;
			}
			++delim_pos;
			if (data_in.size() > delim_pos)
				data_in = data_in.substr(delim_pos, boost::string_ref::npos);
			else
				data_in.clear();
			++i;
		} while (delim_pos && i < 2);

		if (delim_pos != boost::string_ref::npos)
		{
			std::string lib_path(current_lib_path + DYNAMIC_LIBRARY_PATH + lib_name + DYNAMIC_LIBRARY_EXT);

			try
			{
				std::lock_guard<std::mutex> lock(lib_lock_);
				if (!loaded_lib_map_.count(lib_name))
					loaded_lib_map_[lib_name] = std::shared_ptr<library>(new library(lib_path.c_str()));
				return loaded_lib_map_[lib_name]->load_function(func_name.c_str());
			}
			catch (char error_code)
			{
				switch (error_code)
				{
				case E_LIB_NOT_FOUND:
					AEP_LOG_STREAM_SEV(error) << "Library '" << lib_name << "' not found at location '" << lib_path << "'";
					break;
				case E_FUNC_NOT_FOUND:
					AEP_LOG_STREAM_SEV(error) << "Requested function '" << func_name << "' not found in library '" << lib_name << "'";
					break;
				}
			}
		}
		return nullptr;
	}

	char session::export_package(const std::shared_ptr<package> &output_package, const size_t &output_size, char* output_buffer)
	{
		if (output_package->sink_size() <= (size_t)output_size)
		{
			output_package->flush_sink(output_size, output_buffer);
			return SF_GOOD;
		}
		else
		{
			package_output_storage_[output_package.get()] = output_package;
			sprintf(output_buffer, "%p", output_package.get());
			return SF_CHUNK;
		}
	}

	char session::export_next_package(const size_t &output_size, char* output_buffer)
	{
		std::unique_lock<std::mutex> lock(queue_lock_);
		if (package_output_queue_.size() > 0)
		{
			std::shared_ptr<package> output_package = package_output_queue_.front();
			package_output_queue_.pop();
			lock.unlock();
			return export_package(output_package, output_size, output_buffer);
		}
		return SF_NONE;
	}

	session::session()
	{
	}

	session::~session()
	{
	}

	bool session::active()
	{
		return active_;
	}

	void session::process_input(const char* input_data, int output_size, char* output_buffer)
	{
		size_t input_size = strlen(input_data);
		char* return_status = output_buffer;
		++output_buffer;
		--output_size;

		if (input_size)
		{
			const char status = *input_data;
			++input_data;
			boost::string_ref data_in(input_data);

			switch (status)
			{
			case SF_NONE: // Check for new package in queue
				*return_status = export_next_package(output_size, output_buffer);
				break;

			case SF_CHUNK: // Get chunk from package storage
				// TODO
				break;

			case SF_VERSION: // Get extension version information
				strncpy(output_buffer, INF_FILE_VERSION_STR, output_size);
				break;

			case SF_COPY: // Copy input data to output buffer
				strncpy(output_buffer, input_data, output_size);
				break;

			case SF_SYNC: case SF_ASYNC: // Run function from library
				f_export lib_function = pull_lib_function(data_in);

				if (lib_function)
				{
					const bool async = (status == SF_ASYNC);
					std::shared_ptr<package> func_package(new package(input_data));
					handler func_handler(std::shared_ptr<session>(this), func_package, async);

					if (async) // Asynchronous execution
					{
						func_handler.attach_thread(new std::thread(lib_function, func_handler));
						*return_status = SF_HANDLE;
						sprintf(output_buffer, "%p", func_package.get());
					}
					else // Synchronous execution
					{
						lib_function(func_handler);
						*return_status = export_package(func_package, output_size, output_buffer);
					}
				}
				break;
			}
		}
	}

	void session::queue_output(const std::shared_ptr<package> &package_handle)
	{
		std::lock_guard<std::mutex> lock(queue_lock_);
		package_output_queue_.push(package_handle);
	}
}