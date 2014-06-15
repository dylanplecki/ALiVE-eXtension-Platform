
// Program Headers
#include <axp/stdafx.h>
#include <axp/session.h>
#include <axp/package.h>
#include <axp/handler.h>
#include <axp/version.h>
#include <axp/logger_boost.h>
#include <axp/protocol_def.h>

// Key Files
#include <keys/auth.key>

namespace axp
{
	extern std::string current_lib_path; // Defined in <axp/axp.cpp>

	void session::add_to_storage(const std::shared_ptr<package> &package_ref)
	{
		std::lock_guard<std::mutex> lock(storage_lock_);
		auto package_ptr_int = reinterpret_cast<std::uintptr_t>(package_ref.get());

		if (!package_output_storage_.count(package_ptr_int))
			package_output_storage_.emplace(package_ptr_int, package_ref);
	}

	void session::remove_from_storage(const std::uintptr_t &package_ptr_int)
	{
		std::lock_guard<std::mutex> lock(storage_lock_);

		if (package_output_storage_.count(package_ptr_int))
			package_output_storage_.erase(package_ptr_int);
	}

	void session::remove_from_storage(package* package_ptr)
	{
		remove_from_storage(reinterpret_cast<std::uintptr_t>(package_ptr));
	}

	void session::remove_from_storage(const std::shared_ptr<package> &package_ref)
	{
		remove_from_storage(package_ref.get());
	}

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
					AXP_LOG_STREAM_SEV(error) << "Library '" << lib_name << "' not found at location '" << lib_path << "'";
					break;
				case E_FUNC_NOT_FOUND:
					AXP_LOG_STREAM_SEV(error) << "Requested function '" << func_name << "' not found in library '" << lib_name << "'";
					break;
				}
			}
		}
		return nullptr;
	}

	void session::export_address(void* ptr_address, char* output_buffer)
	{
		sprintf(output_buffer, "\"%p\"", ptr_address);
	}

	char session::export_chunk(const char* chunk_addr_str, const size_t &output_size, char* output_buffer)
	{
		try
		{
			std::uintptr_t addr_dec(strtoul(chunk_addr_str, nullptr, 16));
			package* ref_package(reinterpret_cast<package*>(addr_dec));
			
			if (ref_package)
			{
				if (ref_package->flush_sink(output_size, output_buffer, nullptr))
					return SF_CHUNK; // More to pull
				else
					remove_from_storage(addr_dec);
				return SF_GOOD;
			}
			else
			{
				AXP_LOG_STREAM_SEV(warning) << "Cast from stringized pointer address '" << chunk_addr_str << "' returned null";
				return SF_ERROR;
			}
		}
		catch (std::bad_cast& error)
		{
			AXP_LOG_STREAM_SEV(error) << "Bad cast from stringized pointer address '" << chunk_addr_str << "': " << error.what();
			return SF_ERROR;
		}
		return SF_NONE;
	}

	char session::export_package(const std::shared_ptr<package> &output_package, size_t output_size, char* output_buffer)
	{
		if (output_package->sink_size() <= (size_t)output_size)
		{
			output_package->flush_sink(output_size, output_buffer, nullptr);
			remove_from_storage(output_package);
			return SF_GOOD;
		}
		else
		{
			add_to_storage(output_package);
			export_address(output_package.get(), output_buffer);
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
			add_to_storage(output_package);
			export_address(output_package.get(), output_buffer);
			return SF_HANDLE;
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
		#define PREP_RETURN_STATUS ++output_buffer; --output_size

		size_t input_size = strlen(input_data);

		if (input_size)
		{
			const char status = *input_data;
			char* return_status = output_buffer;
			++input_data;
			boost::string_ref data_in(input_data);

			switch (status)
			{
			case SF_NONE: // Check for new package in queue
				PREP_RETURN_STATUS;
				*return_status = export_next_package(output_size, output_buffer);
				break;

			case SF_CHUNK: // Get chunk from package storage (no return status)
				export_chunk(input_data, output_size, output_buffer);
				break;

			case SF_VERSION: // Get extension version information
				strncpy(output_buffer, INF_FILE_VERSION_STR, output_size);
				break;

			case SF_COPY: // Copy input data to output buffer
				strncpy(output_buffer, input_data, output_size);
				break;

			case SF_AUTH: // Get extension authorization key
				strncpy(output_buffer, KEY_AUTH_MAIN, output_size);
				break;

			case SF_TEST: // Create a package and return handle
				PREP_RETURN_STATUS;
				{
					std::shared_ptr<package> func_package(new package(""));
					func_package->write_sink(input_data);
					add_to_storage(func_package);
					*return_status = SF_HANDLE;
					export_address(func_package.get(), output_buffer);
				}
				break;

			case SF_SYNC: case SF_ASYNC: // Run function from library
				PREP_RETURN_STATUS;
				{
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
							export_address(func_package.get(), output_buffer);
						}
						else // Synchronous execution
						{
							lib_function(func_handler);
							*return_status = export_package(func_package, output_size, output_buffer);
						}
					}
					else
						*return_status = SF_ERROR;
				}
				break;
			default:
				PREP_RETURN_STATUS;
				*return_status = SF_ERROR;
			}
		}
	}

	void session::queue_output(const std::shared_ptr<package> &package_handle)
	{
		std::lock_guard<std::mutex> lock(queue_lock_);
		package_output_queue_.push(package_handle);
	}
}