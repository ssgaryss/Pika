#pragma once

#include "Base.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Pika {
	class Log
	{
	public:
		static void Initialize(); // initialize Logger
		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_pCoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_pClientLogger; }; 
	private:
		static std::shared_ptr<spdlog::logger> s_pCoreLogger;	// engine log
		static std::shared_ptr<spdlog::logger> s_pClientLogger; // clienet log
	};
}

//Core log macros
#define PK_CORE_TRACE(...)     ::Pika::Log::getCoreLogger()->trace(__VA_ARGS__)
#define PK_CORE_INFO(...)      ::Pika::Log::getCoreLogger()->info(__VA_ARGS__)
#define PK_CORE_WARN(...)      ::Pika::Log::getCoreLogger()->warn(__VA_ARGS__)
#define PK_CORE_ERROR(...)     ::Pika::Log::getCoreLogger()->error(__VA_ARGS__)
#define PK_CORE_FATAL(...)     ::Pika::Log::getCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define PK_TRACE(...)          ::Pika::Log::getClientLogger()->trace(__VA_ARGS__)
#define PK_INFO(...)           ::Pika::Log::getClientLogger()->info(__VA_ARGS__)
#define PK_WARN(...)           ::Pika::Log::getClientLogger()->warn(__VA_ARGS__)
#define PK_ERROR(...)          ::Pika::Log::getClientLogger()->error(__VA_ARGS__)
#define PK_FATAL(...)          ::Pika::Log::getClientLogger()->critical(__VA_ARGS__)