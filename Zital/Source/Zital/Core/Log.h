#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Zital
{

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return mCoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return mClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> mCoreLogger;
		static std::shared_ptr<spdlog::logger> mClientLogger;

	};

}

//Core logging macros
#define ZT_CORE_TRACE(...) ::Zital::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ZT_CORE_INFO(...)   ::Zital::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ZT_CORE_WARN(...)   ::Zital::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ZT_CORE_ERROR(...) ::Zital::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ZT_CORE_FATAL(...) ::Zital::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client logging macros
#define ZT_TRACE(...)    ::Zital::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ZT_INFO(...)      ::Zital::Log::GetClientLogger()->info(__VA_ARGS__)
#define ZT_WARN(...)      ::Zital::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ZT_ERROR(...)    ::Zital::Log::GetClientLogger()->error(__VA_ARGS__)
#define ZT_FATAL(...)    ::Zital::Log::GetClientLogger()->fatal(__VA_ARGS__)