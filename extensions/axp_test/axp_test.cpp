
// Required Headers
#include <axp\handler.h>


// Function Declarations
extern "C"
{
	__declspec(dllexport) void __stdcall axp_test_function(const axp::handler &handler);
};


// Function Definitions
void __stdcall axp_test_function(const axp::handler &handler)
{
	handler.export_data("\"AXP Test Worked!\"");
}
