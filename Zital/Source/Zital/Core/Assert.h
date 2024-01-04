#pragma once

#include "Zital/Core/Core.h"
#include "Zital/Core/Log.h"
#include <filesystem>


#ifdef ZT_ENABLE_ASSERTS

	//Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	//provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#	define ZT_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { ZT##type##ERROR(msg, __VA_ARGS__); ZT_DEBUGBREAK(); } }
#	define ZT_INTERNAL_ASSERT_WITH_MSG(type, check, ...) ZT_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#	define ZT_INTERNAL_ASSERT_NO_MSG(type, check) ZT_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", ZT_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#	define ZT_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#	define ZT_INTERNAL_ASSERT_GET_MACRO(...) ZT_EXPAND_MACRO( ZT_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, ZT_INTERNAL_ASSERT_WITH_MSG, ZT_INTERNAL_ASSERT_NO_MSG) )

	//Currently accepts at least the condition and one additional parameter (the message) being optional
#	define ZT_ASSERT(...) ZT_EXPAND_MACRO( ZT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#	define ZT_CORE_ASSERT(...) ZT_EXPAND_MACRO( ZT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#	define ZT_ASSERT(...)
#	define ZT_CORE_ASSERT(...)
#endif