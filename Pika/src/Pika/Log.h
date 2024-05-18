#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Pika {
	class PIKA_API Log
	{
	public:
		static void init(); // 初始化Logger
		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return m_pCoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return m_pClientLogger; }; 
	private:
		static std::shared_ptr<spdlog::logger> m_pCoreLogger;	// 引擎端日志
		static std::shared_ptr<spdlog::logger> m_pClientLogger; // 客户端日志
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