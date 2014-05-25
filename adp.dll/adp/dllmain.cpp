// dllmain.cpp : Defines the entry point for the DLL application.

#include "stdafx.h" // Precompiled headers

#include "TranslationUnit.h"


#ifdef _WIN32 // Windows environment

	BOOL APIENTRY DllMain( HMODULE hModule,
						   DWORD  ul_reason_for_call,
						   LPVOID lpReserved
						 )
	{
		switch (ul_reason_for_call)
		{
		case DLL_PROCESS_ATTACH:
			ADP::DynLibLoad();
			break;
		case DLL_THREAD_ATTACH: // Do nothing
		case DLL_THREAD_DETACH: // Do nothing
		case DLL_PROCESS_DETACH:
			ADP::DynLibUnload();
			break;
		}

		return TRUE;
	}

	namespace ADP
	{
		std::string DynLibPath()
		{
			char path_buffer[MAX_PATH];
			HMODULE hm = NULL;

			if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
				GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				(LPCSTR)&DynLibPath,
				&hm))
			{
				int ret = GetLastError();
				BOOST_LOG_TRIVIAL(error) << "GetModuleHandle returned: " << std::to_string(ret);
			}

			GetModuleFileNameA(hm, path_buffer, sizeof(path_buffer));
			boost::string_ref path = path_buffer;

			return path.substr(0, path.find_last_of("\\") + 1).data();
		}
	}

#else // Assume *nix environment

	namespace ADP
	{
		void __attribute__((constructor)) DynLibLoad(void);
		void __attribute__((destructor)) DynLibUnload(void);

		std::string DynLibPath()
		{
			// TODO
		}
	}

#endif // _WIN32


namespace ADP
{
	const std::string dyn_lib_path = DynLibPath();
	TranslationUnit* main_translation_unit; // Singleton class

	void DynLibLoad(void)
	{
		// Boost logging setup
		boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
		boost::log::add_file_log
			(
			boost::log::keywords::file_name = (dyn_lib_path + LOG_FILE),
			boost::log::keywords::format = LOG_FORMAT,
			boost::log::keywords::auto_flush = true
			);
		boost::log::core::get()->set_filter
			(
			boost::log::trivial::severity >= LOG_LEVEL
			);
		boost::log::add_common_attributes();

		// Log standard information
		BOOST_LOG_TRIVIAL(info) << "ALiVE Data Plugin (ADP) Loaded";
		BOOST_LOG_TRIVIAL(info) << "Working Directory: " << dyn_lib_path;

		if (main_translation_unit == nullptr) // Create singleton
			main_translation_unit = new TranslationUnit();
	}

	void DynLibUnload(void)
	{
		if (main_translation_unit != nullptr) // Delete singleton
			delete main_translation_unit;
	}
}
