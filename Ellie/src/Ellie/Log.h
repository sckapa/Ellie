#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Ellie{

	class ELLIE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define EE_CORE_ERROR(...)  Ellie::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EE_CORE_FATAL(...)  Ellie::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define EE_CORE_INFO(...)   Ellie::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EE_CORE_TRACE(...)  Ellie::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EE_CORE_WARN(...)   Ellie::Log::GetCoreLogger()->warn(__VA_ARGS__)

#define EE_ERROR(...)  Ellie::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EE_FATAL(...)  Ellie::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define EE_INFO(...)   Ellie::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EE_TRACE(...)  Ellie::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EE_WARN(...)   Ellie::Log::GetCoreLogger()->warn(__VA_ARGS__)