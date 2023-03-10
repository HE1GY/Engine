#pragma once

#include "spdlog/spdlog.h"

namespace Engine
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return s_CoreLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
#define CORE_TRACE(...)    Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)    Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)    Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...)  Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_ASSERT(x, ...)    if(!x)Engine::Log::GetCoreLogger()->error(__VA_ARGS__); ASSERT(x);

#define TRACE(...)        Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define INFO(...)        Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define WARN(...)        Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ERROR(...)        Engine::Log::GetClientLogger()->error(__VA_ARGS__)

}