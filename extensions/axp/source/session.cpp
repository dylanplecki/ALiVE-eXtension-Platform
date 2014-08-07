
// Program Headers
#include <axp/stdafx.h>
#include <axp/axp.h>
#include <axp/session.h>
#include <axp/package.h>
#include <axp/handler.h>
#include <axp/logger_boost.h>
#include <axp/protocol_def.h>


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


	f_export session::pull_lib_function(const char* &data_in, const size_t &len, const char* &end_ptr)
	{
		size_t cur_pos = 0;
		size_t beg_pos = 0;
		size_t counter = 0;
		std::string lib_name;
		std::string func_name;
		
		for (; cur_pos < len && counter < 2; ++cur_pos)
		{
			if (data_in[cur_pos] == 31) // Unit Separator (UTF+001F &#31)
			{
				switch (counter)
				{
				case 0:
					lib_name.append(data_in + beg_pos, cur_pos - beg_pos);
					break;

				case 1:
					func_name.append(data_in + beg_pos, cur_pos - beg_pos);
					break;
				}

				beg_pos = cur_pos + 1;
				++counter;
			}
		}

		end_ptr = data_in + cur_pos;

		if (lib_name.size() && func_name.size())
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
		std::uintptr_t addr_dec(strtoul(chunk_addr_str, nullptr, 16));
			
		std::lock_guard<std::mutex> lock(storage_lock_);
		if (package_output_storage_.count(addr_dec))
		{
			if (package_output_storage_.at(addr_dec)->flush_sink(output_size, output_buffer, nullptr))
				return SF_CHUNK; // More to pull
			else
			{
				// Remove from storage container
				if (package_output_storage_.count(addr_dec))
					package_output_storage_.erase(addr_dec);
			}

			return SF_GOOD;
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


	session::session() : active_(true)
	{
	}


	session::~session()
	{
	}


	bool session::active()
	{
		return active_;
	}


	void session::set_active(const bool &status)
	{
		active_ = status;
	}


	void session::queue_output(const std::shared_ptr<package> &package_handle)
	{
		std::lock_guard<std::mutex> lock(queue_lock_);
		package_output_queue_.push(package_handle);
	}


	void session::process_input(const char &input_status, const char* input_data, char &output_status, int output_size, char* output_buffer)
	{
		// Macros
		#define SHED_OUTPUT_STATUS --output_buffer; ++output_size // Unsafe method for accessing full output buffer

		// Code
		size_t input_size = strlen(input_data) + 1;

		if (input_size)
		{
			switch (input_status)
			{
			case SF_NONE: // Check for new package in queue
				output_status = export_next_package(output_size, output_buffer);
				break;

			case SF_CHUNK: // Get chunk from package storage (no return status)
				SHED_OUTPUT_STATUS;
				export_chunk(input_data, output_size, output_buffer);
				break;

			case SF_TEST: // Create a package and return handle
				{
					std::shared_ptr<package> func_package(new package(input_data, input_size));
					func_package->write_sink(input_data);
					add_to_storage(func_package);
					output_status = SF_HANDLE;
					export_address(func_package.get(), output_buffer);
				}
				break;

			case SF_INFO: // Return extension status and debug info
				{
					std::string ext_info("[");
					std::unordered_map<std::string, std::string> ext_info_list{
						{ "Session Status", ((active_) ? "true" : "false") },
						{ "Package Queue Count", std::to_string(package_output_queue_.size()) },
						{ "Package Storage Count", std::to_string(package_output_storage_.size()) },
						{ "Loaded Library Count", std::to_string(loaded_lib_map_.size()) },
						{ "Working Directory", ('\"' + current_lib_path + '\"') },
					};

					for (auto ext_info_prop : ext_info_list)
					{
						ext_info.append("[\"");
						ext_info.append(ext_info_prop.first);
						ext_info.append("\",");
						ext_info.append(ext_info_prop.second);
						ext_info.append("],");
					}

					if (ext_info.back() == ',')
						ext_info.back() = ']';
					else
						ext_info.push_back(']');

					strncpy(output_buffer, ext_info.c_str(), output_size);
				}
				output_status = SF_GOOD;
				break;

			case SF_SYNC: case SF_ASYNC: // Run function from library
				{
					const char* arguments;
					f_export lib_function = pull_lib_function(input_data, input_size, arguments);

					if (lib_function)
					{
						const bool async = (input_status == SF_ASYNC);
						std::shared_ptr<package> func_package(new package(arguments, strlen(arguments)));
						handler func_handler(std::shared_ptr<session>(this), func_package, async);

						if (async) // Asynchronous execution
						{
							func_handler.attach_thread(new std::thread(lib_function, func_handler));
							output_status = SF_HANDLE;
							export_address(func_package.get(), output_buffer);
						}
						else // Synchronous execution
						{
							lib_function(func_handler);
							output_status = export_package(func_package, output_size, output_buffer);
						}
					}
					else
					{
						OUTPUT_STRING("Could't load the associated function or library: " + std::string(input_data));
						output_status = SF_ERROR;
					}
				}
				break;

			default:
				const std::string error_message = "Could't find action associated with status flag: " + std::to_string(input_status);
				AXP_LOG_STREAM_SEV(warning) << error_message;
				OUTPUT_STRING(error_message);
				output_status = SF_ERROR;
			}
		}
	}
}