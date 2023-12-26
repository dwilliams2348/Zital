#pragma once

#include <memory>

//platform detection
#ifdef _WIN32
//windows x64/x86
	#ifdef _WIN64
		//windows x64
		#define ZT_PLATFORM_WINDOWS
	#else
		//windows x86
		#error "x86 builds are not supported."
	#endif
#endif

#ifdef ZT_ENABLE_ASSERTS
	#define ZT_ASSERT(x, ...) {if(!(x)) { ZT_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define ZT_CORE_ASSERT(x, ...) {if(!(x)) { ZT_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ZT_ASSERT(x, ...)
	#define ZT_CORE_ASSERT(x, ...)
#endif

#define BIT(x)(1 << x)

#define ZT_BIND_EVENT_FN(fn) [this](auto&&...args)->decltype(auto) {return this->fn(std::forward<decltype(args)>(args)...);}

namespace Zital
{

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}