#include "pkpch.h"
#include "Log.h"

namespace Pika {
	std::shared_ptr<spdlog::logger> Log::s_pCoreLogger;	  
	std::shared_ptr<spdlog::logger> Log::s_pClientLogger; 

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_pCoreLogger = spdlog::stdout_color_mt("Pika");
		s_pCoreLogger->set_level(spdlog::level::trace);
		s_pClientLogger = spdlog::stdout_color_mt("APP");
		s_pClientLogger->set_level(spdlog::level::trace);
	}
}

