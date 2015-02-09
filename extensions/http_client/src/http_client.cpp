
// Program Headers
#include <http_client/stdafx.h>

// API Headers
#include <axp/handler.h>

// Casablanca REST SDK Headers
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>


// Function Declarations
extern "C"
{
	__declspec(dllexport) void __stdcall send_http_request(axp::handler* handler);
};


// Function Definitions
void __stdcall send_http_request(axp::handler* handler)
{
	#define TO_STRING_T(ORIG_STR) utility::conversions::to_string_t(ORIG_STR)
	#define TO_STRING_UTF_8(ORIG_STR) utility::conversions::to_utf8string(ORIG_STR)
	#define MIN_ARGS 2

	ASSERT_SQF_TYPE(handler->input_data(), axp::sqf::array, handler);
	std::vector<axp::sqf::variable> args(handler->input_data().to_vector()); // ["URL", MTD, "qName1", "qData1", ...]

	ASSERT_VECTOR_LEN(args, MIN_ARGS, handler);
	ASSERT_SQF_TYPE(args[0], axp::sqf::string, handler);
	ASSERT_SQF_TYPE(args[1], axp::sqf::scalar, handler);

	utility::string_t url(TO_STRING_T(args[0].to_string()));
	web::http::method api_method(web::http::methods::GET);

	switch (args[1].to_int())
	{
	case 0: api_method = web::http::methods::CONNECT; break;
	case 1: api_method = web::http::methods::DEL; break;
	case 2: api_method = web::http::methods::GET; break;
	case 3: api_method = web::http::methods::HEAD; break;
	case 4: api_method = web::http::methods::MERGE; break;
	case 5: api_method = web::http::methods::OPTIONS; break;
	case 6: api_method = web::http::methods::PATCH; break;
	case 7: api_method = web::http::methods::POST; break;
	case 8: api_method = web::http::methods::PUT; break;
	case 9: api_method = web::http::methods::TRCE; break;
	}

	web::http::client::http_client client(url);
	web::http::uri_builder builder;

	for (size_t i = MIN_ARGS; i < args.size(); i += 2)
	{
		ASSERT_SQF_TYPE(args[i], axp::sqf::string, handler);
		ASSERT_SQF_TYPE(args[i + 1], axp::sqf::string, handler);
		builder.append_query(TO_STRING_T(args[i].to_string()), TO_STRING_T(args[i + 1].to_string()));
	}

	pplx::task<void> req_task = client.request(api_method, builder.to_string()).then([handler](web::http::http_response response)
	{
		try
		{
			auto resp_str_task(response.extract_string());
			handler->export_data(TO_STRING_UTF_8(resp_str_task.get()));
		}
		catch (web::http::http_exception)
		{
			handler->export_data("Invalid content type: Not textual.");
		}
	});

	if (handler->exec_type() == SF_SYNC)
		req_task.wait();
}
