#pragma once

#ifdef ZT_PLATFORM_WINDOWS
#if ZT_DYNAMIC_LINK
	#ifdef ZT_BUILD_DLL
		#define ZITAL_API __declspec(dllexport)
	#else
		#define ZITAL_API __declspec(dllimport)
	#endif
#else
	#define ZITAL_API
#endif
#else
	#error Zital only supports Windows as of now.
#endif

#ifdef ZT_ENABLE_ASSERTS
	#define ZT_ASSERT(x, ...) {if(!(x))} { ZT_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define ZT_CORE_ASSERT(x, ...) {if(!(x))} { ZT_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ZT_ASSERT(x, ...)
	#define ZT_CORE_ASSERT(x, ...)
#endif

#define BIT(x)(1 << x)

#define ZT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)