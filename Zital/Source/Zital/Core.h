#pragma once

#ifdef ZT_PLATFORM_WINDOWS
	#ifdef ZT_BUILD_DLL
		#define ZITAL_API __declspec(dllexport)
	#else
		#define ZITAL_API __declspec(dllimport)
	#endif
#else
	#error Zital only supports Windows as of now.
#endif