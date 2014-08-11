
#if defined(_WIN32) || defined(_WIN64)
	#if defined(AXP_MAIN_DLL)
		#define LibLink __declspec(dllexport)
	#else
		#define LibLink __declspec(dllimport)
	#endif
#elif defined(__linux)

#else
	#error Cannot resolve linkages: OS not supported
#endif
