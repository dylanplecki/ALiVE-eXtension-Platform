
// Program Headers
#include <aep/stdafx.h>
#include <aep/session.h>
#include <aep/package.h>
#include <aep/protocol_def.h>

namespace aep
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