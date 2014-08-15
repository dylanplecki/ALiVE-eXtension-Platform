// axp_testing_console.cpp : Defines the entry point for the console application.
//

// System Defines
#define WIN32_LEAN_AND_MEAN

// System Headers
#include <Windows.h>

// STD Headers
#include <iostream>
#include <stdio.h>
#include <string>

// Program Headers
//#include <axp/handler.h>


// Type Definitions
typedef void(__stdcall *RVExtension_t)(char*, int, const char*);


int main(int argc, char* argv[])
{
	using namespace std;

	// Settings
	const size_t output_buffer_size = 10240;

	const std::string lib_name("axp.dll");
	const std::string func_name("_RVExtension@12");

	// Constants
	const char sf_new_session[] {8};
	const char sf_del_session[] {9};

	// Miscellaneous
	char temp_output_buffer[output_buffer_size];

	wchar_t* lib_name_win = new wchar_t[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, lib_name.c_str(), -1, lib_name_win, MAX_PATH);

	// Load library
	HINSTANCE hGetProcIDDLL = LoadLibrary(lib_name_win);

	if (!hGetProcIDDLL) {
		std::cout << "ERROR: Could not load the dynamic library @" << lib_name << std::endl;
		return EXIT_FAILURE;
	}

	// Load main RVE function
	RVExtension_t RVExtension;
	void* func_addr = GetProcAddress(hGetProcIDDLL, func_name.c_str());

	if (!func_addr) {
		std::cout << "ERROR: Could not locate the function - " << func_name << std::endl;
		return EXIT_FAILURE;
	}
	else
		RVExtension = (RVExtension_t)func_addr;

	// Initialize library
	RVExtension(temp_output_buffer, output_buffer_size, sf_new_session);

	while (true) {
		cout << "\n\n";

		string input_data("");
		char output_buffer[output_buffer_size];

		char status_code;
		cout << "Status Code [1,14]: ";
		scanf("%hhu", &status_code);
		cout << "\n\n";

		input_data.push_back(status_code);

		std::string enter_type;
		cout << "Enter SQF [v]ariable(s), raw [d]ata, or [n]othing? [v/d/n]: ";
		cin >> enter_type;
		cout << "\n\n";

		switch (enter_type[0])
		{
		case 'v':
			{
				string var_status;
				do {
					char sqf_var_type;
					cout << "SQF Variable Type [1,16]: ";
					scanf("%hhu", &sqf_var_type);
					cout << "\n";

					string sqf_var_data;
					cout << "Enter Variable Data:\n\n";
					cin >> sqf_var_data;
					cout << "\n---\n\n";

					input_data.push_back(sqf_var_type);
					input_data.append(sqf_var_data);

					cout << "Add another SQF Variable? [y/n]: ";
					cin >> var_status;
					cout << "\n\n---\n\n";
				} while (tolower(var_status[0]) == 'y');
			}
			break;

		case 'd':
			{
				string raw_data;
				cout << "Enter Raw Data:\n\n";
				cin >> raw_data;
				cout << "\n---\n\n";
				input_data.append(raw_data);
			}
			break;
		}

		cout << "Input Data: " << input_data << "\n\n";
		cout << "Processing...\n\n";

		RVExtension(output_buffer, output_buffer_size, input_data.c_str());

		cout << "Output:\n";
		cout << "    Length: " << strlen(output_buffer) << "\n";
		cout << "    Status: " << std::to_string(output_buffer[0]) << "\n";
		cout << "\n\n" << output_buffer << "\n\n";

		system("pause");
		cout << "\n\n-------------------------------------------------\n";
	}

	// Close library
	RVExtension(temp_output_buffer, output_buffer_size, sf_del_session);

	return EXIT_SUCCESS;
}

