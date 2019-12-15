#pragma once

#include "spdlog/spdlog.h"

#include <memory.h>

//SOURCE: https://github.com/gabime/spdlog

namespace Muse
{
	class Log
	{
	public:
		Log() = default;
		~Log() = default;

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger()
		{
			return ms_EngineLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetApplicationLogger()
		{
			return ms_ApplicationLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger> ms_EngineLogger;
		static std::shared_ptr<spdlog::logger> ms_ApplicationLogger;

	};
}

// Engine log macros
#define LOG_ENGINE_ERROR(...)	::Muse::Log::GetEngineLogger()->error(__VA_ARGS__)
#define LOG_ENGINE_WARN(...)	::Muse::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define LOG_ENGINE_INFO(...)	::Muse::Log::GetEngineLogger()->info(__VA_ARGS__)
#define LOG_ENGINE_TRACE(...)	::Muse::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define LOG_ENGINE_FATAL(...)	::Muse::Log::GetEngineLogger()->fatal(__VA_ARGS__)

// Application log macros
#define LOG_ERROR(...)		::Muse::Log::GetApplicationLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)		::Muse::Log::GetApplicationLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)		::Muse::Log::GetApplicationLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)		::Muse::Log::GetApplicationLogger()->trace(__VA_ARGS__)
#define LOG_FATAL(...)		::Muse::Log::GetApplicationLogger()->fatal(__VA_ARGS__)
