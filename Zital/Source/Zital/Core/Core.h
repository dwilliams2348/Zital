#pragma once

#include <memory>

#include "Zital/Core/PlatformDetection.h"

#ifdef ZT_DEBUG
#	if defined(ZT_PLATFORM_WINDOWS)
#		define ZT_DEBUGBREAK() __debugbreak()
#	elif defined ZT_PLATFORM_LINUX
#		include <signal.h>
#		define ZT_DEBUGBREAK() raise(SIGTRAP)
#endif
#	define ZT_ENABLE_ASSERTS
#else
#	define ZT_DEBUGBREAK()
#endif

#define ZT_EXPAND_MACRO(x) x
#define ZT_STRINGIFY_MACRO(x) #x

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

#include "Zital/Core/Log.h"
#include "Zital/Core/Assert.h"