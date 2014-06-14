
// Program Headers
#include <adp/stdafx.h>
#include <adp/session.h>
#include <adp/package.h>
#include <adp/protocol_def.h>

namespace adp
{
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

	void session::process_input(const char* input_data, int outputSize, char* output_buffer)
	{
		// TODO
	}

	void session::queue_output(const std::shared_ptr<package> &package_handle)
	{
		// TODO
	}
}